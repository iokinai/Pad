#include <QFuture>
#include <QFutureWatcher>
#include <pages/editor/codenode.hpp>
#include <pages/editor/editorcontroller.hpp>
#include <pages/editor/imagenode.hpp>
#include <pages/editor/node.hpp>
#include <pages/editor/note.hpp>
#include <pages/editor/textnode.hpp>
#include <storage/notescache.hpp>

namespace pad {

EditorController::EditorController(Note *currentNote, NotesCache *cache,
                                   QObject *parent)
    : QObject(parent), _currentNote(currentNote), _cache(cache) {}

Note *EditorController::currentNote() { return _currentNote; }

void EditorController::setCurrentNote(Note *note) {
  auto task = _cache->loadMediaForNoteIfRequired(note);

  QCoro::connect(std::move(task), this, [this](bool result) {
    if (result) {
      emit imageLoaded();
    }
  });

  _currentNote = note;
  emit currentNoteChanged();
}

} // namespace pad
