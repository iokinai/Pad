#include <pages/editor/imagenode.hpp>

namespace pad {

ImageNode::ImageNode(const QString &src, QObject *parent)
    : Node(parent), _src(src) {}

Node::NodeType ImageNode::type() const noexcept { return Node::Image; }

const QString &ImageNode::src() const { return _src; }

} // namespace pad
