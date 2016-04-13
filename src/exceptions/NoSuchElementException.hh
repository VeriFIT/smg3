#pragma once

#include <string>
#include <stdexcept>

namespace smg {

class NoSuchElementException : public std::runtime_error {
 public:
  explicit NoSuchElementException(const std::string& message) noexcept
    : runtime_error(message) { }
  explicit NoSuchElementException(const char* message) noexcept
    : runtime_error(message) { }
};

}  // namespace smg
