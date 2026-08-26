#include <exceptions/couldnotsavefile.hpp>
#include <exceptions/invalidfile.hpp>
#include <storage/manifest.hpp>
#include <storage/markup.hpp>
#include <storage/storagecontroller.hpp>

#include <minizip-ng/mz.h>
#include <minizip-ng/mz_strm.h>
#include <minizip-ng/mz_zip.h>
#include <minizip-ng/mz_zip_rw.h>

#include <QCoro/QCoroFuture>
#include <QDir>
#include <QStandardPaths>
#include <QtConcurrent/QtConcurrent>

namespace pad {

StorageController::StorageController(MediaStorage *storage, QObject *parent)
    : QObject(parent), _storage(storage) {}

static QByteArray readZipEntryToBuffer(void *reader) {
  mz_zip_file *file_info = nullptr;
  if (mz_zip_reader_entry_get_info(reader, &file_info) != MZ_OK) {
    return {};
  }

  if (mz_zip_reader_entry_open(reader) != MZ_OK) {
    return {};
  }

  QByteArray buffer;
  buffer.resize(static_cast<qsizetype>(file_info->uncompressed_size));

  int32_t bytesRead =
      mz_zip_reader_entry_read(reader, buffer.data(), buffer.size());
  mz_zip_reader_entry_close(reader);

  if (bytesRead < 0) {
    return {};
  }

  buffer.resize(bytesRead);
  return buffer;
}

QString StorageController::makeNoteName() const noexcept {
  return "Pad-Note-" +
         QString::number(QDateTime::currentDateTimeUtc().toMSecsSinceEpoch());
}

LoadNotesResult StorageController::loadNotes(const int start,
                                             const int maxLoadCount) {
  QDir dir(_basicPath, {"*.pad"}, QDir::Name | QDir::Reversed);
  LoadNotesResult loadResult;

  if (!dir.exists()) {
    dir.mkpath(_basicPath);
    return {};
  }

  QStringList files = dir.entryList(QDir::Files);
  loadResult.totalCount = files.size();

  if (start >= files.size()) {
    return loadResult;
  }

  loadResult.notes.reserve(maxLoadCount);

  for (size_t i = start; i < qMin(start + maxLoadCount, files.size()); ++i) {
    auto apath = dir.absoluteFilePath(files[i]);
    auto result = loadSingleNote(apath);

    if (result == std::nullopt)
      continue;

    result->name = files[i];

    loadResult.notes.emplaceBack(std::move(*result));
  }

  return loadResult;
}

std::optional<LoadNoteResult>
StorageController::loadSingleNote(const QString &path) {
  void *reader = mz_zip_reader_create();
  if (!reader) {
    return std::nullopt;
  }

  struct Cleanup {
    void *r;
    ~Cleanup() {
      mz_zip_reader_close(r);
      mz_zip_reader_delete(&r);
    }
  } cleanup{reader};

  if (mz_zip_reader_open_file(reader, path.toUtf8().constData()) != MZ_OK) {
    return std::nullopt;
  }

  try {
    Manifest m = loadAndParseManifest(reader);
    Note *note = loadAndParseContent(reader, m);
    auto media = extractMediaWithDataFromArchive(reader);
    return LoadNoteResult{note, media};
  } catch (const std::exception &e) {
    // LOG exception
  } catch (...) {
    // LOG exception
  }

  return std::nullopt;
}

Manifest StorageController::loadAndParseManifest(void *reader) {
  if (mz_zip_reader_locate_entry(reader, "manifest.json", 1) != MZ_OK) {
    throw InvalidFile("manifest.json not found");
  }

  QByteArray data = readZipEntryToBuffer(reader);
  if (data.isEmpty()) {
    throw InvalidFile("manifest.json is empty or failed to read");
  }

  QJsonDocument doc = QJsonDocument::fromJson(data);
  if (!doc.isObject()) {
    throw InvalidFile("manifest.json is not a valid JSON object");
  }

  return Manifest::fromJson(doc.object());
}

Note *StorageController::loadAndParseContent(void *reader,
                                             const Manifest &manifest) {
  if (mz_zip_reader_locate_entry(reader, "markup.json", 1) != MZ_OK) {
    throw InvalidFile("markup.json not found");
  }

  QByteArray data = readZipEntryToBuffer(reader);
  if (data.isEmpty()) {
    throw InvalidFile("markup.json is empty or failed to read");
  }

  QJsonDocument doc = QJsonDocument::fromJson(data);
  if (!doc.isObject()) {
    throw InvalidFile("markup.json is not a valid JSON object");
  }

  return buildNoteFromJson(doc.object(), manifest);
}

QVector<QPair<QString, QByteArray>>
StorageController::extractMediaWithDataFromArchive(void *reader) {
  QVector<QPair<QString, QByteArray>> media;

  if (mz_zip_reader_goto_first_entry(reader) != MZ_OK) {
    return {};
  }

  do {
    mz_zip_file *file_info = nullptr;
    if (mz_zip_reader_entry_get_info(reader, &file_info) != MZ_OK) {
      continue;
    }

    QString fileName = QString::fromUtf8(file_info->filename);

    if (!fileName.startsWith("media/") || fileName.endsWith("/")) {
      continue;
    }

    QByteArray imageBytes = readZipEntryToBuffer(reader);
    if (imageBytes.isEmpty()) {
      continue;
    }

    QString imageName = QFileInfo(fileName).fileName();
    media.emplace_back(imageName, imageBytes);

  } while (mz_zip_reader_goto_next_entry(reader) == MZ_OK);

  return media;
}

void StorageController::saveNoteToPad(NoteSaveData data) {
  void *writer = mz_zip_writer_create();
  if (!writer) {
    throw CouldNotSaveFile();
  }

  struct Cleanup {
    void *w;
    ~Cleanup() {
      mz_zip_writer_close(w);
      mz_zip_writer_delete(&w);
    }
  } cleanup{writer};

  if (mz_zip_writer_open_file(writer,
                              (_basicPath + data.path).toUtf8().constData(), 0,
                              0) != MZ_OK) {
    throw CouldNotSaveFile();
  }

  auto writeEntry = [writer](const QString &entryName,
                             const QByteArray &bytes) {
    mz_zip_file file_info = {};
    file_info.filename = entryName.toUtf8().constData();
    file_info.compression_method = MZ_COMPRESS_METHOD_DEFLATE;

    if (mz_zip_writer_entry_open(writer, &file_info) != MZ_OK) {
      return false;
    }

    int32_t written =
        mz_zip_writer_entry_write(writer, bytes.constData(), bytes.size());
    mz_zip_writer_entry_close(writer);

    return written == bytes.size();
  };

  if (!writeEntry("manifest.json", QJsonDocument(data.manifest).toJson())) {
    throw CouldNotSaveFile();
  }

  if (!writeEntry("markup.json", QJsonDocument(data.markup).toJson())) {
    throw CouldNotSaveFile();
  }

  for (const auto &media : data.mediaData) {
    if (!writeEntry("media/" + media.first, media.second)) {
      throw CouldNotSaveFile();
    }
  }
}

NoteSaveData
StorageController::prepareNoteSaveData(Note *note,
                                       QHash<QString, Note *> &notePathMap) {
  QString path = notePathMap.key(note);

  if (path.isEmpty()) {
    path = makeNoteName() + ".pad";
    notePathMap.insert(path, note);
  }

  Manifest m(note->createdAt());
  QJsonObject markup = buildJsonForNote(note);
  auto media = extractMediaFromNote(note);

  return NoteSaveData{path, m.toJson(), markup,
                      loadMediaWithDataFromStorage(media)};
}

QStringList StorageController::extractMediaFromNote(Note *note) {
  QStringList media;

  for (const auto &node : note->nodes()) {
    if (node->type() == Node::Image) {
      auto imageNode = dynamic_cast<ImageNode *>(node);
      if (imageNode) {
        media << imageNode->src();
      }
    }
  }
  return media;
}

QVector<QPair<QString, QByteArray>>
StorageController::loadMediaWithDataFromStorage(const QStringList &mediaPaths) {
  QVector<QPair<QString, QByteArray>> media;
  for (const auto &path : mediaPaths) {
    QByteArray data = _storage->getMedia(path);
    media.emplaceBack(path, data);
  }
  return media;
}

QCoro::Task<LoadNotesResult>
StorageController::loadNotesAsync(const int start, const int maxLoadCount) {
  co_return co_await QtConcurrent::run([this, start, maxLoadCount]() {
    auto loadedNotes = loadNotes(start, maxLoadCount);

    for (const auto &note : loadedNotes.notes) {
      if (qApp) {
        note.loadedNote->moveToThread(qApp->thread());
      }
    }

    return loadedNotes;
  });
}

QCoro::Task<void> StorageController::saveNoteAsync(NoteSaveData data) {
  co_await QtConcurrent::run([this, data]() { return saveNoteToPad(data); });
}

} // namespace pad
