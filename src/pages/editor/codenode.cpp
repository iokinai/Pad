#include <pages/editor/codenode.hpp>

namespace pad {

CodeNode::CodeNode(const QString &code, QObject *parent)
    : Node(parent), _code(code) {}

Node::NodeType CodeNode::type() const noexcept { return Node::Code; }

QString CodeNode::code() const { return _code; }

void CodeNode::setCode(const QString &code) {
  if (_code == code) {
    return;
  }
  _code = code;
  emit codeChanged();
  emit edited();
}

} // namespace pad
