#include "IllegalArgumentException.hh"

namespace smg {

IllegalArgumentException::IllegalArgumentException(char const* const message) noexcept
    : std::exception() {
  msg_ = message;
}

const char* IllegalArgumentException::what() const noexcept { return msg_; }

}  // namespace smg
