#include <configure.hpp>
#include <exceptions/invalidmanifest.hpp>
#include <storage/manifest.hpp>

namespace pad {

Manifest::Manifest(const QDateTime &createdAt)
    : _applicationName(PAD_PROJECT), _createdAt(createdAt) {}

Manifest Manifest::now() noexcept {
  QDateTime createdAt = QDateTime::currentDateTime();
  return Manifest(createdAt);
}

QJsonObject Manifest::toJson() const {
  QJsonObject object;
  object["applicationName"] = _applicationName;
  object["createdAt"] = _createdAt.toString(Qt::ISODate);
  return object;
}

bool Manifest::validateObject(const QJsonObject &json) {
  return json.contains("applicationName") && json.contains("createdAt");
}

Manifest Manifest::fromJson(const QJsonObject &json) {
  if (!Manifest::validateObject(json)) {
    throw InvalidManifest();
  }

  QString applicationName = json["applicationName"].toString();

  if (applicationName != PAD_PROJECT) {
    throw InvalidManifest();
  }

  QDateTime createdAt =
      QDateTime::fromString(json["createdAt"].toString(), Qt::ISODate);
  return Manifest(createdAt);
}

QString Manifest::applicationName() const { return _applicationName; }

QDateTime Manifest::createdAt() const { return _createdAt; }

} // namespace pad
