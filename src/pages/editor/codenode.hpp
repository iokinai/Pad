#pragma once

#include <QString>
#include <pages/editor/node.hpp>

namespace pad {

class CodeNode : public Node {
  Q_OBJECT

  QString _code;

  Q_PROPERTY(QString code READ code WRITE setCode NOTIFY codeChanged)

public:
  CodeNode(const QString &code, QObject *parent = nullptr);
  NodeType type() const noexcept override;

  QString code() const;
  void setCode(const QString &code);

signals:
  void codeChanged();
};

} // namespace pad
