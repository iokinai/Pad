#include <pages/editor/textnode.hpp>

namespace pad {

TextNode::TextNode(const QString &text, QObject *parent)
    : Node(parent), _text(text) {}

Node::NodeType TextNode::type() const noexcept { return Node::Text; }

const QString &TextNode::text() const noexcept { return _text; }

void TextNode::setText(const QString &newText) noexcept {
  if (_text == newText) {
    return;
  }

  _text = newText;
  emit textChanged();
  emit edited();
}

} // namespace pad
