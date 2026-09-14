#pragma once

#include <QObject>

namespace pad {

class Matcher : public QObject {
  Q_OBJECT
public:
  explicit Matcher(QObject *parent = nullptr);
  virtual bool match(const QStringView &s1, const QStringView &s2) = 0;
};

} // namespace pad
