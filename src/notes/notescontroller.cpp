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
#include <storage/notescache.hpp>
#include <utils.hpp>

namespace pad {

void NotesController::addEmptyNote() {
  auto note = _cache->addEmptyNote();
  _unsavedNotes.insert(note);
  note->setHasUnsavedChanges(true);
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

NotesController::NotesController(NotesCache *cache, QObject *parent)
    : QObject(parent), _cache(cache), _notesModel({}), _totalCount(0),
      _loaded(0) {
  connect(&_notesModel, &NotesModel::noteAdded, this,
          &NotesController::onNoteAdded);
  connect(_cache, &NotesCache::fullNoteLoaded, this,
          &NotesController::onFullNoteLoaded);
  connect(_cache, &NotesCache::totalCountChanged, this,
          &NotesController::onTotalCountChanged);
  connect(_cache, &NotesCache::loaded, this, [this]() {
    _cache->requestFullyLoadedNotesAsync(_loaded, LOAD_PACKET_LENGTH)
        .then([]() { qDebug() << "result got"; });
  });

  _cache->loadNotesAsync().then([]() { qDebug() << "requested"; });
}

void NotesController::onFullNoteLoaded(QVector<Note *> notes) {
  for (auto *note : notes) {
    prepareAndPushCreatedNote(note);
  }
}

void NotesController::onTotalCountChanged() {
  _totalCount = _cache->totalCount();
  emit notesCountChanged();
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

  co_await _cache->requestFullyLoadedNotesAsync(_loaded, LOAD_PACKET_LENGTH);

  setLoadingNotes(false);
}

QCoro::Task<void> NotesController::saveNoteAsync(Note *note) {

  note->setHasUnsavedChanges(false);
  _unsavedNotes.remove(note);

  try {
    co_await _cache->saveNoteAsync(note);
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
  auto name = _cache->addMediaFromSystem(systemPath);

  if (name.isEmpty()) {
    emit addImageError(systemPath);
  }

  return name;
}

} // namespace pad
