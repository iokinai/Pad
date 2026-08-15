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

  void insertText(Node *node, const QString &text, bool above = true);

  Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
  Q_PROPERTY(QDateTime createdAt MEMBER _createdAt CONSTANT)
public:
  enum Roles { NodeRole = Qt::UserRole + 1 };

  Note(const QString &title, QVector<Node *> &&nodes, QDateTime createdAt,
       QObject *parent = nullptr);

  virtual int rowCount(const QModelIndex &parent = {}) const override;
  virtual QVariant data(const QModelIndex &index, int role) const override;
  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE void insertTextAbove(Node *top, const QString &text = "");
  Q_INVOKABLE void insertTextBelow(Node *bottom, const QString &text = "");

  QString title() const;
  void setTitle(const QString &title);

signals:
  void titleChanged();

private slots:
  void onNodeRemoveRequested();
  void onNoteEdited();
};

} // namespace pad
