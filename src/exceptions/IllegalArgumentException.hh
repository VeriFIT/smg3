#include <exception>

namespace smg {

class IllegalArgumentException : public std::exception {
 private:
  char const* msg_ = nullptr;

 public:
  explicit IllegalArgumentException(char const* const message) noexcept;
  const char* what() const noexcept override;
};

}  // namespace smg
