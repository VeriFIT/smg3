#pragma once

#include <stdexcept>
#include <string>

namespace smg {

class UnsupportedOperationException : public std::runtime_error {
 public:
  explicit UnsupportedOperationException(const std::string& message) noexcept
    : runtime_error(message) { }
  explicit UnsupportedOperationException(const char* message) noexcept
    : runtime_error(message) { }
};

}  // namespace smg
