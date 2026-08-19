#include <exceptions/invalidmanifest.hpp>

namespace pad {

InvalidManifest::InvalidManifest() : _message("Invalid manifest read") {}

const char *InvalidManifest::what() const noexcept { return _message.c_str(); }

} // namespace pad
