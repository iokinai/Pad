#include <pages/editor/codenode.hpp>
#include <pages/editor/imagenode.hpp>
#include <pages/editor/note.hpp>
#include <pages/editor/textnode.hpp>

namespace pad {

void Note::connectNode(Node *node) {
  connect(node, &Node::removeRequested, this, &Note::onNodeRemoveRequested);
  connect(node, &Node::edited, this, &Note::onNodeEdited);
}

Note::Note(const QString &title, QVector<Node *> &&nodes, QDateTime createdAt,
           MediaStorage *storage, QObject *parent)
    : QAbstractListModel(parent), _title(title), _nodes(std::move(nodes)),
      _createdAt(createdAt), _storage(storage) {
  for (auto *node : _nodes) {
    node->setParent(this);
    connectNode(node);
  }

  connect(this, &Note::titleChanged, this, &Note::onNodeEdited);
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

  emit nodesCountChanged();
  emit noteEdited();
}

void Note::onNodeEdited() { emit noteEdited(); }

void Note::insertAnyNode(Node *basic, Node *insert, bool above) {
  auto index = _nodes.indexOf(basic);

  if (index == -1)
    index = _nodes.size();
  else if (!above)
    ++index;

  beginInsertRows({}, index, index);

  connectNode(insert);

  _nodes.insert(index, insert);

  endInsertRows();

  emit nodesCountChanged();
  emit noteEdited();
}

void Note::insertText(Node *node, const QString &text, bool above) {
  insertAnyNode(node, new TextNode(text, this), above);
}

void Note::insertImage(Node *node, const QString &path, bool above) {
  QString imageName = _storage->addMediaFromSystem(path);

  if (imageName.isEmpty()) {
    emit addImageError(path);
    return;
  }

  insertAnyNode(node, new ImageNode(imageName, this), above);
}

void Note::insertCode(Node *node, const QString &code, bool above) {
  insertAnyNode(node, new CodeNode(code, this), above);
}

void Note::insertTextAbove(Node *top, const QString &text) {
  insertText(top, text, true);
}

void Note::insertTextBelow(Node *bottom, const QString &text) {
  insertText(bottom, text, false);
}

void Note::insertImageAbove(Node *top, const QString &path) {
  insertImage(top, path, true);
}

void Note::insertImageBelow(Node *bottom, const QString &path) {
  insertImage(bottom, path, false);
}

void Note::insertCodeAbove(Node *top, const QString &code) {
  insertCode(top, code, true);
}

void Note::insertCodeBelow(Node *bottom, const QString &code) {
  insertCode(bottom, code, false);
}

QString Note::title() const { return _title; }

void Note::setTitle(const QString &title) {
  if (_title == title)
    return;

  _title = title;
  emit titleChanged();
  emit onNodeEdited();
}

int Note::nodesCount() const { return _nodes.count(); }

const QVector<Node *> &Note::nodes() const { return _nodes; }

void Note::unsafeSetNodes(QVector<Node *> &&nodes) {
  _nodes = std::move(nodes);

  for (const auto node : _nodes)
    connectNode(node);
}

QDateTime Note::createdAt() const { return _createdAt; }

bool Note::hasUnsavedChanges() const { return _hasUnsavedChanges; }

void Note::setHasUnsavedChanges(bool hasUnsavedChanges) {
  if (_hasUnsavedChanges == hasUnsavedChanges)
    return;

  _hasUnsavedChanges = hasUnsavedChanges;
  emit hasUnsavedChangesChanged();
}

} // namespace pad
