#include "UnsupportedOperationException.hh"

UnsupportedOperationException::UnsupportedOperationException(char const* const _Message) noexcept
  :std::exception()
{
  msg = _Message;
}

const char * UnsupportedOperationException::what() const noexcept
{
  return msg;
}
