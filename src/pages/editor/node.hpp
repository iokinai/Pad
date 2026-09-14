#pragma once

#include <QObject>

namespace pad {

class Node : public QObject {
  Q_OBJECT

public:
  enum NodeType {
    Text,
    Image,
    Code,
  };

  Q_ENUM(NodeType)

  Q_PROPERTY(NodeType type READ type CONSTANT)

  Node(QObject *parent = nullptr);

  virtual NodeType type() const noexcept = 0;

signals:
  void removeRequested();
  void edited();
};

} // namespace pad
