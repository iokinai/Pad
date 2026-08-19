#pragma once

#include <exception>
#include <string>

namespace pad {

class InvalidManifest : public std::exception {
public:
  InvalidManifest();
  const char *what() const noexcept override;

private:
  std::string _message;
};

} // namespace pad
