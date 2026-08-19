#pragma once

#include <stdexcept>

namespace pad {

class CouldNotSaveFile : public std::runtime_error {
public:
  CouldNotSaveFile() : std::runtime_error("Could not save file") {}
};

} // namespace pad
