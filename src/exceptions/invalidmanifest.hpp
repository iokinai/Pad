#pragma once

#include <stdexcept>

namespace pad {

class InvalidManifest : public std::runtime_error {
public:
  inline InvalidManifest() : std::runtime_error("Invalid manifest read") {}
};

} // namespace pad
