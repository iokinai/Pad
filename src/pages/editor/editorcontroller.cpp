#include <pages/editor/editorcontroller.hpp>
#include <pages/editor/imagenode.hpp>
#include <pages/editor/node.hpp>
#include <pages/editor/note.hpp>
#include <pages/editor/textnode.hpp>

namespace pad {

static QVector<Node *> getTestNodes() {
  QVector<Node *> nodes;

  nodes.push_back(new TextNode("123"));
  nodes.push_back(new TextNode("456"));
  nodes.push_back(new ImageNode(
      "file:///C:\\Users\\MegaD\\Downloads\\Новий проєкт (1).png"));

  return nodes;
}

EditorController::EditorController(QObject *parent)
    : QObject(parent), _note(new Note("Untitled", std::move(getTestNodes()),
                                      QDateTime::currentDateTime(), this)) {}

QAbstractListModel *EditorController::note() const noexcept { return _note; }

} // namespace pad
