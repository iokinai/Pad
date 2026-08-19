#pragma once

#include <QString>
#include <stdexcept>

namespace pad {

class InvalidJsonNodeType : public std::runtime_error {
public:
  InvalidJsonNodeType(const QString &nodeType)
      : std::runtime_error("Invalid node type: " + nodeType.toStdString()) {}
};

} // namespace pad
