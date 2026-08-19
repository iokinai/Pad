#include <utils.hpp>

#include <QUuid>

namespace pad::utils {

QString randomUuid() {
  return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

} // namespace pad::utils
