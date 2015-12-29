#include "IllegalArgumentException.hh"

IllegalArgumentException::IllegalArgumentException(char const* const _Message) noexcept
: std::exception() {
    msg = _Message;
}

const char * IllegalArgumentException::what() const noexcept {
    return msg;
}
