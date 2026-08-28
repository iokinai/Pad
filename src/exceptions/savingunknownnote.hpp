#pragma once

#include <stdexcept>

namespace pad {

class SavingUnknownNote : public std::runtime_error {
public:
  inline SavingUnknownNote()
      : std::runtime_error("Trying to save unknown note") {}
};

} // namespace pad
