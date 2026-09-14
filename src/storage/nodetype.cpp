#include <exceptions/invalidjsonnodetype.hpp>
#include <storage/nodetype.hpp>

namespace pad {

QString nodeTypeIntoJsonNodeType(const Node::NodeType type) {
  switch (type) {
  case Node::Text:
    return "text";
  case Node::Image:
    return "image";
  case Node::Code:
    return "code";
  }
}

Node::NodeType nodeTypeFromJsonNodeType(const QString &nodeType) {
  if (nodeType == "text") {
    return Node::Text;
  } else if (nodeType == "image") {
    return Node::Image;
  } else if (nodeType == "code") {
    return Node::Code;
  }

  throw InvalidJsonNodeType(nodeType);
}

} // namespace pad
