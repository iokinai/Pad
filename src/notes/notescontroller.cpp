#include <QDateTime>
#include <notes/notescontroller.hpp>
#include <notes/notesmodel.hpp>
#include <pages/editor/codeNode.hpp>
#include <pages/editor/imageNode.hpp>
#include <pages/editor/textnode.hpp>

namespace pad {

static QVector<Node *> getTestNodes() {
  QVector<Node *> nodes;

  nodes.push_back(new TextNode("123"));
  nodes.push_back(new TextNode("456"));
  nodes.push_back(new ImageNode(
      "file:///C:\\Users\\MegaD\\Downloads\\Новий проєкт (1).png"));
  nodes.push_back(
      new CodeNode("#include <iostream>\n\nint main() {\n\tstd::cout << "
                   "\"Hello World\" << std::endl;\n}"));

  return nodes;
}

static QVector<Note *> getTestNotes() {
  QVector<Note *> notes;

  notes.push_back(
      new Note("test1", getTestNodes(), QDateTime::currentDateTime()));
  notes.push_back(
      new Note("test2", getTestNodes(), QDateTime::currentDateTime()));
  notes.push_back(
      new Note("test3", getTestNodes(), QDateTime::currentDateTime()));
  notes.push_back(
      new Note("test4", getTestNodes(), QDateTime::currentDateTime()));
  notes.push_back(
      new Note("test5", getTestNodes(), QDateTime::currentDateTime()));

  return std::move(notes);
}

NotesController::NotesController(QObject *parent)
    : QObject(parent), _notesModel(std::move(getTestNotes())), _totalCount(5),
      _loaded(5) {
  connect(&_notesModel, &NotesModel::noteAdded, this,
          &NotesController::onNoteAdded);
}

size_t NotesController::notesCount() const { return _totalCount; }

NotesModel *NotesController::notes() { return &_notesModel; }

void NotesController::onNoteAdded() {
  ++_totalCount;
  ++_loaded;
  emit notesCountChanged();
}

} // namespace pad
