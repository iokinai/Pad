#include <minizip-ng/mz.h>
#include <minizip-ng/mz_strm.h>
#include <minizip-ng/mz_zip.h>
#include <minizip-ng/mz_zip_rw.h>

#include <QDateTime>
#include <QDir>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>
#include <QtMath>
#include <exceptions/couldnotsavefile.hpp>
#include <exceptions/invalidfile.hpp>
#include <notes/notescontroller.hpp>
#include <notes/notesmodel.hpp>
#include <pages/editor/codeNode.hpp>
#include <pages/editor/imageNode.hpp>
#include <pages/editor/textnode.hpp>
#include <storage/markup.hpp>
#include <utils.hpp>

using namespace pad::detail;

namespace pad {

LoadNotesResult NotesController::loadNotes() {
  QDir dir(basicPath);
  LoadNotesResult loadResult;

  if (!dir.exists()) {
    dir.mkpath(basicPath);
    return {};
  }

  QStringList files = dir.entryList({"*.pad"}, QDir::Files);
  loadResult.totalCount = files.size();
  loadResult.notes.reserve(loadResult.totalCount);

  for (size_t i = 0; i < qMin(LOAD_FIRST_COUNT, files.size()); ++i) {
    auto apath = dir.absoluteFilePath(files[i]);
    auto result = loadSingleNote(apath);

    if (result == std::nullopt)
      continue;

    result->name = apath;

    loadResult.notes.push_back(result.value());
  }

  return loadResult;
}

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

Manifest NotesController::loadAndParseManifest(void *reader) {
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

Note *NotesController::loadAndParseContent(void *reader,
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

  return buildNoteFromJson(doc.object(), manifest, _storage);
}

QVector<QPair<QString, QByteArray>>
NotesController::extractMediaWithDataFromArchive(void *reader,
                                                 MediaStorage *storage) {
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

std::optional<LoadNoteResult>
NotesController::loadSingleNote(const QString &path) {
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
    auto media = extractMediaWithDataFromArchive(reader, _storage);
    return LoadNoteResult{note, media};
  } catch (const std::exception &e) {
    // LOG exception
  } catch (...) {
    // LOG exception
  }

  return std::nullopt;
}

QVector<QPair<QString, QByteArray>>
NotesController::loadMediaWithDataFromStorage(const QStringList &mediaPaths) {
  QVector<QPair<QString, QByteArray>> media;
  for (const auto &path : mediaPaths) {
    QByteArray data = _storage->getMedia(path);
    media.append({path, data});
  }
  return media;
}

NoteSaveData NotesController::prepareNoteSaveData(Note *note) {
  QString path = _notePathMap.key(note);

  if (path.isEmpty()) {
    path = basicPath + makeNoteName() + ".pad";
    _notePathMap.insert(path, note);
  }

  Manifest m(note->createdAt());
  QJsonObject markup = buildJsonForNote(note);
  auto media = extractMediaFromNote(note);

  return NoteSaveData{path, m.toJson(), markup,
                      loadMediaWithDataFromStorage(media)};
}

QStringList NotesController::extractMediaFromNote(Note *note) {
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

void NotesController::saveNoteToPad(
    const QString &path, const QJsonObject &manifest, const QJsonObject &markup,
    const QVector<QPair<QString, QByteArray>> &medias) {
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

  if (mz_zip_writer_open_file(writer, path.toUtf8().constData(), 0, 0) !=
      MZ_OK) {
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

  if (!writeEntry("manifest.json", QJsonDocument(manifest).toJson())) {
    throw CouldNotSaveFile();
  }

  if (!writeEntry("markup.json", QJsonDocument(markup).toJson())) {
    throw CouldNotSaveFile();
  }

  for (const auto &media : medias) {
    if (!writeEntry("media/" + media.first, media.second)) {
      throw CouldNotSaveFile();
    }
  }
}

void NotesController::addEmptyNote() {
  auto note =
      new Note("", {}, QDateTime::currentDateTime(), _storage, &_notesModel);
  ++_totalCount;
  prepareAndPushCreatedNote(note);
  saveNoteAsync(note);
}

QString NotesController::makeNoteName() {
  return "Pad-Note-" + utils::randomUuid();
}

void NotesController::onNoteEdited() {
  Note *senderNote = qobject_cast<Note *>(sender());

  if (!_unsavedNotes.contains(senderNote)) {
    _unsavedNotes.insert(senderNote);
    senderNote->setHasUnsavedChanges(true);
  }
}

void NotesController::prepareAndPushCreatedNote(Note *note) {
  connect(note, &Note::noteEdited, this, &NotesController::onNoteEdited);
  _notesModel.pushNote(note);
}

NotesController::NotesController(MediaStorage *storage, QObject *parent)
    : QObject(parent), _notesModel({}), _totalCount(0), _loaded(0),
      _storage(storage) {
  connect(&_notesModel, &NotesModel::noteAdded, this,
          &NotesController::onNoteAdded);

  loadNotesAsync();
}

size_t NotesController::notesCount() const { return _totalCount; }

NotesModel *NotesController::notes() { return &_notesModel; }

void NotesController::onNoteAdded() {
  ++_loaded;
  emit notesCountChanged();
}

void NotesController::loadNotesAsync() {
  auto watcher = new QFutureWatcher<LoadNotesResult>();

  connect(watcher, &QFutureWatcher<LoadNotesResult>::finished, this,
          [watcher, this]() {
            auto results = watcher->result();
            _totalCount = results.totalCount;

            for (const auto &result : results.notes) {
              result.loadedNote->setParent(&_notesModel);
              _notePathMap[result.name] = result.loadedNote;
              prepareAndPushCreatedNote(result.loadedNote);

              for (const auto &media : result.medias) {
                _storage->addMedia(media.first, media.second);
              }
            }

            watcher->deleteLater();
          });

  watcher->setFuture(QtConcurrent::run([this]() {
    auto results = loadNotes();

    for (const auto &result : results.notes) {
      if (qApp) {
        result.loadedNote->moveToThread(qApp->thread());
      }
    }

    return results;
  }));
}

void NotesController::saveNoteAsync(Note *note) {
  auto saveData = prepareNoteSaveData(note);
  note->setHasUnsavedChanges(false);
  _unsavedNotes.remove(note);

  auto *watcher = new QFutureWatcher<bool>();

  connect(watcher, &QFutureWatcher<bool>::finished, this,
          [watcher, note, this]() {
            auto result = watcher->result();

            if (!result) {
              note->setHasUnsavedChanges(true);
              _unsavedNotes.insert(note);
              emit couldNotSaveNote();
            }

            watcher->deleteLater();
          });

  watcher->setFuture(QtConcurrent::run([this, saveData]() {
    try {
      saveNoteToPad(saveData.path, saveData.manifest, saveData.markup,
                    saveData.mediaData);
      return true;
    } catch (...) {
      return false;
    }
  }));
}

} // namespace pad
