#pragma once

#include <QAbstractListModel>
#include <QDateTime>
#include <QObject>
#include <QString>
#include <QVector>
#include <pages/editor/node.hpp>

namespace pad {

class Note : public QAbstractListModel {
  Q_OBJECT

  QString _title;
  QVector<Node *> _nodes;
  QDateTime _createdAt;

  void connectNode(Node *node);

  void insertAnyNode(Node *basic, Node *insert, bool above = true);
  void insertText(Node *node, const QString &text, bool above = true);
  void insertImage(Node *node, const QString &path, bool above = true);
  void insertCode(Node *node, const QString &code, bool above = true);

  Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
  Q_PROPERTY(QDateTime createdAt MEMBER _createdAt CONSTANT)
  Q_PROPERTY(int nodesCount READ nodesCount NOTIFY nodesCountChanged)
public:
  enum Roles { NodeRole = Qt::UserRole + 1 };

  Note(const QString &title, QVector<Node *> &&nodes, QDateTime createdAt,
       QObject *parent = nullptr);

  virtual int rowCount(const QModelIndex &parent = {}) const override;
  virtual QVariant data(const QModelIndex &index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE void insertTextAbove(Node *top, const QString &text = "");
  Q_INVOKABLE void insertTextBelow(Node *bottom, const QString &text = "");
  Q_INVOKABLE void insertImageAbove(Node *top, const QString &path);
  Q_INVOKABLE void insertImageBelow(Node *bottom, const QString &path);
  Q_INVOKABLE void insertCodeAbove(Node *top, const QString &code = "");
  Q_INVOKABLE void insertCodeBelow(Node *bottom, const QString &code = "");

  QString title() const;
  void setTitle(const QString &title);
  int nodesCount() const;

signals:
  void titleChanged();
  void nodesCountChanged();

private slots:
  void onNodeRemoveRequested();
  void onNoteEdited();
};

} // namespace pad
