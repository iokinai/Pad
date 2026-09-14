#pragma once

#include <QJsonObject>

namespace pad {

class IJsonObjectSerializable {
public:
  virtual ~IJsonObjectSerializable() = default;
  virtual QJsonObject toJson() const = 0;
};

} // namespace pad
