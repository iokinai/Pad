#include <pages/editor/codenode.hpp>
#include <pages/editor/editorcontroller.hpp>
#include <pages/editor/imagenode.hpp>
#include <pages/editor/node.hpp>
#include <pages/editor/note.hpp>
#include <pages/editor/textnode.hpp>

namespace pad {

EditorController::EditorController(Note *currentNote, QObject *parent)
    : QObject(parent), _currentNote(currentNote) {}

Note *EditorController::currentNote() { return _currentNote; }

void EditorController::setCurrentNote(Note *note) {
  _currentNote = note;
  emit currentNoteChanged();
}

} // namespace pad
