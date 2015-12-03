
class UnsupportedOperationException : public std::exception
{
public:
  explicit UnsupportedOperationException(char const* const _Message);
  char const* what() const override;
};
