#pragma once

#include <QObject>
#include <QString>
#include <pages/editor/node.hpp>

namespace pad {

class TextNode : public Node {
  Q_OBJECT

  QString _text;

  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
  TextNode(const QString &text, QObject *parent = nullptr);

  virtual NodeType type() const noexcept override;

  const QString &text() const noexcept;
  void setText(const QString &newText) noexcept;

signals:
  void textChanged();
};

} // namespace pad
