#include "UnsupportedOperationException.hh"

namespace smg {

UnsupportedOperationException::UnsupportedOperationException(char const* const message) noexcept
    : std::exception() {
  msg_ = message;
}

const char* UnsupportedOperationException::what() const noexcept { return msg_; }

}  // namespace smg
