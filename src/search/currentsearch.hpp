#pragma once

#include <atomic>

namespace pad {

struct CurrentSearch {
  std::atomic_bool cancelled;
};

} // namespace pad
