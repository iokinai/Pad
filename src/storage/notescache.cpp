#include <exceptions/savingunknownnote.hpp>
#include <storage/loadednote.hpp>
#include <storage/notescache.hpp>

#include <algorithm>
#include <ranges>

#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>
#include <qcoro/QCoroFuture>

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
  LoadNotesResult result = co_await QtConcurrent::run([this]() {
    auto result = _storageController->loadNotesWithoutMedia();
    for (const auto &loadedNote : result.notes) {
      if (qApp) {
        loadedNote.note->moveToThread(qApp->thread());
      }
    }

    return result;
  });

  qDebug() << "before:" << result.notes.isDetached();

  auto it = result.notes.begin();

  qDebug() << "after:" << result.notes.isDetached();

  // qDebug() << result.totalCount;
  // qDebug() << result.notes.size();

  // qDebug() << "got notes";

  // for (const LoadedNote &note : std::as_const(result.notes)) {
  //   qDebug() << note.path;
  // }

  //_totalCount = result.totalCount;
  // emit totalCountChanged();

  // result.notes.detach();
  // for (const auto &note : result.notes) {
  // note.note->setParent(this);
  //_notes.push_back(note);
  //}

  // emit anyNoteLoaded();
  // emit loaded();
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

  emit fullNoteLoaded(loadedNotes);
}

QString NotesCache::makeNoteName() const noexcept {
  return "Pad-Note-" +
         QString::number(QDateTime::currentDateTimeUtc().toMSecsSinceEpoch());
}

Note *NotesCache::addEmptyNote() {
  auto note = new Note("", {}, QDateTime::currentDateTime(), this);
  auto name = makeNoteName();

  _notes.push_back({name, note, LoadedNoteType::FullyLoaded});
  emit totalCountChanged();
  emit fullNoteLoaded({note});
  return note;
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

} // namespace pad
