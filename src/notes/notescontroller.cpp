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

void NotesController::loadNotesAsync() {
  auto watcher = new QFutureWatcher<LoadNotesResult>();

  setLoadingNotes(true);

  connect(watcher, &QFutureWatcher<LoadNotesResult>::finished, this,
          [watcher, this]() {
            auto results = watcher->result();
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
            watcher->deleteLater();
          });

  watcher->setFuture(QtConcurrent::run([this]() {
    auto results = _storageController->loadNotes(_loaded, LOAD_PACKET_LENGTH);

    for (const auto &result : results.notes) {
      if (qApp) {
        result.loadedNote->moveToThread(qApp->thread());
      }
    }

    return results;
  }));
}

bool NotesController::loadingNotes() const noexcept { return _loadingNotes; }

void NotesController::setLoadingNotes(bool v) noexcept {
  if (_loadingNotes == v) {
    return;
  }

  _loadingNotes = v;
  emit loadingNotesChanged();
}

size_t NotesController::loadedNotesCount() const noexcept { return _loaded; }

void NotesController::saveNoteAsync(Note *note) {
  auto saveData = _storageController->prepareNoteSaveData(note, _notePathMap);
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
