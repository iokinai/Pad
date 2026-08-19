#pragma once

#include <QString>
#include <pages/editor/node.hpp>

namespace pad {

QString nodeTypeIntoJsonNodeType(const Node::NodeType type);
Node::NodeType nodeTypeFromJsonNodeType(const QString &nodeType);

} // namespace pad
