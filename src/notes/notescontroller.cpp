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
#include <search/searchcontroller.hpp>
#include <storage/markup.hpp>
#include <storage/notescache.hpp>
#include <utils.hpp>

namespace pad {

void NotesController::addEmptyNote() { _cache->createEmptyNote(); }

void NotesController::onNoteCreated(Note *note) {
  _unsavedNotes.insert(note);
  note->setHasUnsavedChanges(true);
  prepareAndPushCreatedNote(note, true);
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
    _notesModel.pushLoadedNoteNoteFront(note);
  } else {
    _notesModel.pushLoadedNoteBack(note);
  }
}

NotesController::NotesController(NotesCache *cache,
                                 SearchController *searchController,
                                 QObject *parent)
    : QObject(parent), _cache(cache), _searchController(searchController),
      _notesModel({}), _totalCount(0), _loaded(0) {
  connect(&_notesModel, &NotesModel::noteAdded, this,
          &NotesController::onNoteAdded);
  connect(_cache, &NotesCache::fullNotesLoaded, this,
          &NotesController::onFullNotesLoaded);
  connect(_cache, &NotesCache::noteCreated, this,
          &NotesController::onNoteCreated);
  connect(_cache, &NotesCache::totalCountChanged, this,
          &NotesController::onTotalCountChanged);
  connect(_cache, &NotesCache::loaded, this, [this]() {
    _cache->requestFullyLoadedNotesAsync(_loaded, LOAD_PACKET_LENGTH);
  });
  connect(_searchController, &SearchController::searchEnded, this,
          &NotesController::onSearchEnded);

  connect(this, &NotesController::searchQueryChanged, this,
          &NotesController::onSearchQueryChanged);

  QMetaObject::invokeMethod(
      this, [this]() { _cache->loadNotesAsync(); }, Qt::QueuedConnection);
}

void NotesController::onSearchEnded(QVector<LoadedNote> notes) {
  QVector<Note *> foundNotes;
  foundNotes.reserve(notes.size());

  for (const auto &loaded : notes) {
    foundNotes.push_back(loaded.note);
  }

  _notesModel.swapTo(std::move(foundNotes));
}

void NotesController::onSearchQueryChanged() {
  if (_searchQuery.isEmpty()) {
    _notesModel.swapTo(std::nullopt);
  } else {
    QMetaObject::invokeMethod(
        this, [this]() { _searchController->search(_searchQuery); },
        Qt::QueuedConnection);
  }
}

void NotesController::onFullNotesLoaded(QVector<Note *> notes) {
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

const QString &NotesController::searchQuery() const noexcept {
  return _searchQuery;
}

void NotesController::setSearchQuery(const QString &newQuery) {
  if (newQuery == _searchQuery) {
    return;
  }

  _searchQuery = newQuery;
  emit searchQueryChanged();
}

} // namespace pad
