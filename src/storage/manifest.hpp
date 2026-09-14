#pragma once

#include <QDateTime>
#include <QString>
#include <storage/ijsonobjectserializable.hpp>

namespace pad {

class Manifest : public IJsonObjectSerializable {
  QString _applicationName;
  QDateTime _createdAt;

  static bool validateObject(const QJsonObject &json);

public:
  Manifest(const QDateTime &createdAt);

  QString applicationName() const;
  QDateTime createdAt() const;

  static Manifest now() noexcept;
  static Manifest fromJson(const QJsonObject &json);
  QJsonObject toJson() const override;
};

} // namespace pad
