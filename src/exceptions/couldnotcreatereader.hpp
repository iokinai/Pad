#pragma once

#include <QString>
#include <stdexcept>

namespace pad {

class CouldNotCreateReader : public std::runtime_error {
public:
  inline CouldNotCreateReader(const QString &path)
      : std::runtime_error(QString("Could not create reader for file: %1")
                               .arg(path)
                               .toStdString()) {}
};

} // namespace pad
