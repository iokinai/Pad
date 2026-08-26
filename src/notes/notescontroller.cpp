#include <QCoro/QCoroQml>
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
#include <pages/editor/codenode.hpp>
#include <pages/editor/imagenode.hpp>
#include <pages/editor/textnode.hpp>
#include <storage/markup.hpp>
#include <utils.hpp>

namespace pad {

void NotesController::addEmptyNote() {
  auto note = new Note("", {}, QDateTime::currentDateTime(), &_notesModel);
  ++_totalCount;
  prepareAndPushCreatedNote(note, true);
  saveNoteAsync(note);
}

void NotesController::onNoteEdited() {
  Note *senderNote = qobject_cast<Note *>(sender());

  if (!_unsavedNotes.contains(senderNote)) {
    _unsavedNotes.insert(senderNote);
    senderNote->setHasUnsavedChanges(true);
  }
}

void NotesController::prepareAndPushCreatedNote(Note *note, bool front) {
  connect(note, &Note::noteEdited, this, &NotesController::onNoteEdited);

  if (front) {
    _notesModel.pushNoteFront(note);
  } else {
    _notesModel.pushNoteBack(note);
  }
}

NotesController::NotesController(MediaStorage *storage,
                                 StorageController *storageController,
                                 QObject *parent)
    : QObject(parent), _storageController(storageController), _notesModel({}),
      _totalCount(0), _loaded(0), _storage(storage) {
  connect(&_notesModel, &NotesModel::noteAdded, this,
          &NotesController::onNoteAdded);

  loadNotesAsync();
}

size_t NotesController::notesCount() const { return _totalCount; }

NotesModel *NotesController::notes() { return &_notesModel; }

void NotesController::onNoteAdded() {
  ++_loaded;
  emit notesCountChanged();
  emit loadedNotesCountChanged();
}

QCoro::Task<void> NotesController::loadNotesAsync() {
  setLoadingNotes(true);

  auto results =
      co_await _storageController->loadNotesAsync(_loaded, LOAD_PACKET_LENGTH);

  _totalCount = results.totalCount;

  for (const auto &result : results.notes) {
    result.loadedNote->setParent(&_notesModel);
    _notePathMap[result.name] = result.loadedNote;

    for (const auto &media : result.medias) {
      _storage->addMedia(media.first, media.second);
    }

    prepareAndPushCreatedNote(result.loadedNote);
  }

  setLoadingNotes(false);
}

QCoro::Task<void> NotesController::saveNoteAsync(Note *note) {
  auto saveData = _storageController->prepareNoteSaveData(note, _notePathMap);
  note->setHasUnsavedChanges(false);
  _unsavedNotes.remove(note);

  try {
    co_await _storageController->saveNoteAsync(saveData);
  } catch (...) {
    note->setHasUnsavedChanges(true);
    _unsavedNotes.insert(note);
    emit couldNotSaveNote();
  }
}

QCoro::QmlTask NotesController::qmlSaveNoteAsync(Note *note) {
  return saveNoteAsync(note);
}

QCoro::QmlTask NotesController::qmlLoadNotesAsync() { return loadNotesAsync(); }

bool NotesController::loadingNotes() const noexcept { return _loadingNotes; }

void NotesController::setLoadingNotes(bool v) noexcept {
  if (_loadingNotes == v) {
    return;
  }

  _loadingNotes = v;
  emit loadingNotesChanged();
}

size_t NotesController::loadedNotesCount() const noexcept { return _loaded; }

QString NotesController::loadImageFromSystem(const QString &systemPath) {
  auto name = _storage->addMediaFromSystem(systemPath);

  if (name.isEmpty()) {
    emit addImageError(systemPath);
  }

  return name;
}

} // namespace pad
