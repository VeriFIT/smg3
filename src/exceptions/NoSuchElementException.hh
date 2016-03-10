/*
    Created by Viktor Malik on 23.2.2016.
*/

#pragma once

#include <exception>

namespace smg {

class NoSuchElementException : public std::exception {
 private:
  char const* msg_ = nullptr;

 public:
  explicit NoSuchElementException(char const* const message) noexcept;
  const char* what() const noexcept override;
};

}  // namespace smg
