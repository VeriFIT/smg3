#pragma once

#include <string>
#include <stdexcept>

namespace smg {

class IllegalArgumentException : public std::runtime_error {
 public:
  explicit IllegalArgumentException(const std::string& message) noexcept
    : runtime_error(message) { } ;
  explicit IllegalArgumentException(const char* message) noexcept
    : runtime_error(message) { } ;
};

}  // namespace smg
