#include <stdexcept>

namespace pad {

class InvalidJson : public std::runtime_error {
public:
  InvalidJson(const std::string &message) : std::runtime_error(message) {}
};

} // namespace pad
