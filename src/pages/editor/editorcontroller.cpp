#include <pages/editor/editorcontroller.hpp>
#include <pages/editor/node.hpp>
#include <pages/editor/note.hpp>
#include <pages/editor/textnode.hpp>

namespace pad {

static QVector<Node *> getNodes() {
  QVector<Node *> nodes;

  nodes.push_back(new TextNode("123"));
  nodes.push_back(new TextNode("456"));

  return nodes;
}

EditorController::EditorController(QObject *parent)
    : QObject(parent), _note(new Note("Untitled", std::move(getNodes()),
                                      QDateTime::currentDateTime(), this)) {}

QAbstractListModel *EditorController::note() const noexcept { return _note; }

} // namespace pad
