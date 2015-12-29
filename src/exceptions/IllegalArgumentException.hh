#include <exception>

class IllegalArgumentException: public std::exception {
private:
    char const* msg = nullptr;
public:
    explicit IllegalArgumentException(char const* const _Message) noexcept;
    const char * what() const noexcept override;
};
