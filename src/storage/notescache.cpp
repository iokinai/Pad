#include <exception>
#include <exceptions/savingunknownnote.hpp>
#include <storage/loadednote.hpp>
#include <storage/notescache.hpp>

#include <algorithm>
#include <ranges>

#include <QCoro/QCoroFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>

using namespace std::chrono_literals;

namespace pad {

NotesCache::NotesCache(StorageController *storageController, QObject *parent)
    : QObject(parent), _storageController(storageController) {}

const QVector<LoadedNote> &NotesCache::allNotes() const { return _notes; }

QVector<Note *> NotesCache::fullyLoadedNotes() const {
  QVector<Note *> result;

  std::ranges::copy(
      _notes | std::views::filter([](const auto &ln) {
        return ln.type == LoadedNoteType::FullyLoaded;
      }) | std::views::transform([](const auto &ln) { return ln.note; }),
      std::back_inserter(result));

  return result;
}

size_t NotesCache::totalCount() const { return _totalCount; }

QCoro::Task<void> NotesCache::loadNotesAsync() {
  LoadNotesResult *result = co_await QtConcurrent::run([this]() {
    auto result = _storageController->loadNotesWithoutMedia();

    for (const auto &loadedNote : result->notes) {
      if (qApp) {
        loadedNote.note->moveToThread(qApp->thread());
      }
    }

    return result;
  });

  _totalCount = result->totalCount;
  emit totalCountChanged();

  for (const auto &note : result->notes) {
    note.note->setParent(this);
    _notes.push_back(note);
  }

  delete result;

  emit anyNoteLoaded();
  emit loaded();
}

QCoro::Task<void> NotesCache::requestFullyLoadedNotesAsync(int since,
                                                           int count) {
  if (since >= _notes.size()) {
    co_return;
  }

  const size_t end = qMin(since + count, _notes.size());

  QVector<QString> pathsToLoad;
  QVector<Note *> loadedNotes;
  for (size_t i = since; i < end; ++i) {
    if (_notes[i].type != LoadedNoteType::FullyLoaded) {
      pathsToLoad.push_back(_notes[i].path);
    }
  }

  if (!pathsToLoad.isEmpty()) {
    co_await QtConcurrent::run([this, pathsToLoad]() {
      for (const auto &path : pathsToLoad) {
        _storageController->loadMediaFor(path);
      }
    });

    for (size_t i = since; i < end; ++i) {
      _notes[i].type = LoadedNoteType::FullyLoaded;
      loadedNotes.push_back(_notes[i].note);
    }
  }

  emit fullNotesLoaded(loadedNotes);
}

QString NotesCache::makeNoteName() const noexcept {
  return "Pad-Note-" +
         QString::number(QDateTime::currentDateTimeUtc().toMSecsSinceEpoch()) +
         ".pad";
}

void NotesCache::createEmptyNote() {
  auto note = new Note("", {}, QDateTime::currentDateTime(), this);
  auto name = makeNoteName();

  _notes.push_back({name, note, LoadedNoteType::FullyLoaded});
  ++_totalCount;
  emit totalCountChanged();
  emit noteCreated(note);
}

QCoro::Task<void> NotesCache::saveNoteAsync(Note *note) {
  QString path;
  auto it =
      std::find_if(_notes.begin(), _notes.end(), [note](auto &loadedNote) {
        return loadedNote.note == note;
      });

  if (it == _notes.end()) {
    throw SavingUnknownNote();
  }

  path = it->path;

  auto saveData = _storageController->prepareNoteSaveData(note, path);
  co_await _storageController->saveNoteAsync(saveData);
}

QString NotesCache::addMediaFromSystem(const QString &systemPath) {
  return _storageController->addMediaFromSystem(systemPath);
}

QCoro::Task<bool> NotesCache::loadMediaForNoteIfRequired(Note *note) {
  auto it =
      std::find_if(_notes.cbegin(), _notes.cend(),
                   [note](const LoadedNote &ln) { return ln.note == note; });

  if (it == _notes.end() || it->type == LoadedNoteType::FullyLoaded) {
    co_return false;
  }

  const QString path = it->path;

  try {
    co_await QtConcurrent::run(
        [this, path]() { _storageController->loadMediaFor(path); });
  } catch (const std::exception &e) {
    qDebug() << "error while loading media for note:" << e.what();
    co_return false;
  }

  auto loaded =
      std::find_if(_notes.begin(), _notes.end(),
                   [note](const LoadedNote &ln) { return ln.note == note; });

  if (loaded != _notes.end())
    loaded->type = LoadedNoteType::FullyLoaded;

  co_return true;
}

} // namespace pad
