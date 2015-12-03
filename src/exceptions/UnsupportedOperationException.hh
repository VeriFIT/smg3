#include <exception>

class UnsupportedOperationException : public std::exception
{
private:
  char const* msg = nullptr;
public:
  explicit UnsupportedOperationException(char const* const _Message) noexcept;
  const char * what() const noexcept override;
};
