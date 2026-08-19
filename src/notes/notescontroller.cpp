#include <minizip-ng/mz.h>
#include <minizip-ng/mz_strm.h>
#include <minizip-ng/mz_zip.h>
#include <minizip-ng/mz_zip_rw.h>

#include <QDateTime>
#include <QDir>
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

namespace pad {

void NotesController::loadNotes() {
  QDir dir(basicPath);

  if (!dir.exists()) {
    dir.mkpath(basicPath);
    return;
  }

  QStringList files = dir.entryList({"*.pad"}, QDir::Files);
  _totalCount = files.size();
  emit notesCountChanged();

  for (size_t i = 0; i < qMin(LOAD_FIRST_COUNT, files.size()); ++i) {
    auto apath = dir.absoluteFilePath(files[i]);
    Note *note = loadSingleNote(apath);

    if (note == nullptr)
      continue;
    _notePathMap[apath] = note;
  }
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

void NotesController::registerMedia(void *reader, MediaStorage *storage) {
  if (mz_zip_reader_goto_first_entry(reader) != MZ_OK) {
    return;
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
    storage->addMedia(imageName, imageBytes);

  } while (mz_zip_reader_goto_next_entry(reader) == MZ_OK);
}

Note *NotesController::loadSingleNote(const QString &path) {
  void *reader = mz_zip_reader_create();
  if (!reader) {
    return nullptr;
  }

  struct Cleanup {
    void *r;
    ~Cleanup() {
      mz_zip_reader_close(r);
      mz_zip_reader_delete(&r);
    }
  } cleanup{reader};

  if (mz_zip_reader_open_file(reader, path.toUtf8().constData()) != MZ_OK) {
    return nullptr;
  }

  try {
    Manifest m = loadAndParseManifest(reader);
    Note *note = loadAndParseContent(reader, m);
    registerMedia(reader, _storage);

    note->setParent(&_notesModel);
    prepareAndPushCreatedNote(note);
    return note;
  } catch (const std::exception &e) {
    // LOG exception
  } catch (...) {
    // LOG exception
  }

  return nullptr;
}

void NotesController::saveNote(Note *note) {
  QString path = _notePathMap.key(note);

  if (path.isEmpty()) {
    path = basicPath + makeNoteName() + ".pad";
    _notePathMap.insert(path, note);
  }

  Manifest m(note->createdAt());
  QJsonObject markup = buildJsonForNote(note);
  auto media = extractMediaFromNote(note);
  saveNoteToPad(path, m.toJson(), markup, media);
  _unsavedNotes.remove(note);
  note->setHasUnsavedChanges(false);
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

void NotesController::saveNoteToPad(const QString &path,
                                    const QJsonObject &manifest,
                                    const QJsonObject &markup,
                                    const QStringList &mediaPaths) {
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

  for (const QString &mediaPath : mediaPaths) {
    QByteArray data = _storage->getMedia(mediaPath);
    if (!writeEntry("media/" + mediaPath, data)) {
      throw CouldNotSaveFile();
    }
  }
}

void NotesController::addEmptyNote() {
  auto note =
      new Note("", {}, QDateTime::currentDateTime(), _storage, &_notesModel);
  ++_totalCount;
  saveNote(note);
  prepareAndPushCreatedNote(note);
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

  loadNotes();
}

size_t NotesController::notesCount() const { return _totalCount; }

NotesModel *NotesController::notes() { return &_notesModel; }

void NotesController::onNoteAdded() {
  ++_loaded;
  emit notesCountChanged();
}

} // namespace pad
