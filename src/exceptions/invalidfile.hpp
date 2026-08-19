#pragma once

#include <QString>
#include <stdexcept>

class InvalidFile : public std::runtime_error {
public:
  InvalidFile(const QString &message)
      : std::runtime_error(message.toStdString()) {}
};
