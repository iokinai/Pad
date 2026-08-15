#pragma once

#include <QString>
#include <pages/editor/node.hpp>

namespace pad {

class ImageNode : public Node {
  Q_OBJECT

  QString _src;

  Q_PROPERTY(QString src READ src CONSTANT)

public:
  ImageNode(const QString &src, QObject *parent = nullptr);
  NodeType type() const noexcept override;

  const QString &src() const;
};

} // namespace pad
