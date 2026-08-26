#include <search/notescache.hpp>

#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>
#include <qcoro/QCoroFuture>

using namespace std::chrono_literals;

namespace pad {

NotesCache::NotesCache(StorageController *storageController, QObject *parent)
    : QObject(parent), _storageController(storageController),
      _timer(new QTimer(this)) {
  _timer->setSingleShot(true);
  connect(_timer, &QTimer::timeout, this, &NotesCache::clearCache);
}

QFuture<void> NotesCache::loadNotesToCacheAsync() {
  if (_loaded) {
    return QtFuture::makeReadyVoidFuture();
  }

  return QtConcurrent::run(
             [this]() { return _storageController->loadNotes().notes; })
      .then(this, [this](QVector<LoadNoteResult> notes) {
        _notes.reserve(_notes.size() + notes.size());

        for (const auto &note : notes) {
          if (note.loadedNote) {
            note.loadedNote->setParent(this);
            _notes.push_back(note.loadedNote);
          }
        }

        _loaded = true;
      });
}

QCoro::Task<QVector<Note *>> NotesCache::notesAsync() {
  if (!_loaded) {
    co_await loadNotesToCacheAsync();
  }

  _timer->start(5min);

  co_return _notes;
}

void NotesCache::clearCache() {
  qDeleteAll(_notes);
  _loaded = false;
}

} // namespace pad
