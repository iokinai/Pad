#include <pages/editor/note.hpp>
#include <pages/editor/textnode.hpp>

namespace pad {

void Note::connectNode(Node *node) {
  connect(node, &Node::removeRequested, this, &Note::onNodeRemoveRequested);
  connect(node, &Node::edited, this, &Note::onNoteEdited);
}

Note::Note(const QString &title, QVector<Node *> &&nodes, QDateTime createdAt,
           QObject *parent)
    : QAbstractListModel(parent), _title(title), _nodes(std::move(nodes)),
      _createdAt(createdAt) {
  for (auto *node : _nodes) {
    node->setParent(this);
    connectNode(node);
  }

  connect(this, &Note::titleChanged, this, &Note::onNoteEdited);
}

int Note::rowCount(const QModelIndex &parent) const { return _nodes.count(); }

QVariant Note::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return {};

  if (role != Note::NodeRole)
    return {};

  return QVariant::fromValue(_nodes[index.row()]);
}

QHash<int, QByteArray> Note::roleNames() const { return {{NodeRole, "node"}}; }

void Note::onNodeRemoveRequested() {
  auto *node = qobject_cast<Node *>(sender());

  if (!node)
    return;

  const int index = _nodes.indexOf(node);

  beginRemoveRows({}, index, index);

  _nodes.removeAt(index);

  endRemoveRows();

  node->deleteLater();
}

void Note::onNoteEdited() {}

void Note::insertText(Node *node, const QString &text, bool above) {
  auto index = _nodes.indexOf(node);

  if (index == -1)
    index = _nodes.size();
  else if (!above)
    ++index;

  beginInsertRows({}, index, index);

  auto ins = new TextNode(text, this);

  connectNode(ins);

  _nodes.insert(index, ins);

  endInsertRows();
}

void Note::insertTextAbove(Node *top, const QString &text) {
  insertText(top, text, true);
}

void Note::insertTextBelow(Node *bottom, const QString &text) {
  insertText(bottom, text, false);
}

QString Note::title() const { return _title; }

void Note::setTitle(const QString &title) {
  if (_title == title)
    return;

  _title = title;
  emit titleChanged();
  emit onNoteEdited();
}

} // namespace pad
