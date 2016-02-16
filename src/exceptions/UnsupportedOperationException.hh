#include <exception>

namespace smg {

class UnsupportedOperationException : public std::exception {
 private:
  char const* msg_ = nullptr;

 public:
  explicit UnsupportedOperationException(char const* const message) noexcept;
  const char* what() const noexcept override;
};

}  // namespace smg
