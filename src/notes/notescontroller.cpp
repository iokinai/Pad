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
    auto results = _storageController->loadNotes(LOAD_FIRST_COUNT);

    for (const auto &result : results.notes) {
      if (qApp) {
        result.loadedNote->moveToThread(qApp->thread());
      }
    }

    return results;
  }));
}

void NotesController::saveNoteAsync(Note *note) {
  QString path = _notePathMap.key(note);

  if (path.isEmpty()) {
    path = makeNoteName() + ".pad";
    _notePathMap.insert(path, note);
  }

  auto saveData = _storageController->prepareNoteSaveData(note, path);
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
      _storageController->saveNoteToPad(saveData);
      return true;
    } catch (...) {
      return false;
    }
  }));
}

} // namespace pad
