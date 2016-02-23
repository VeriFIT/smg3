/*
    Created by Viktor Malik on 23.2.2016.
*/

#include "NoSuchElementException.hh"

namespace smg {

NoSuchElementException::NoSuchElementException(char const* const message) noexcept
    : std::exception() {
  msg_ = message;
}

const char* smg::NoSuchElementException::what() const noexcept { return msg_; }

}  // namespace smg
