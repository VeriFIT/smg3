#pragma once

#include <string>

namespace smg {

class SMGValue {
 private:
  static long id_counter_;
  static const SMGValue NULL_VALUE;
  static const SMGValue INVALID_VALUE;
  static const SMGValue UNKNOWN_VALUE;
  long id_;

  explicit SMGValue(const long id);

 public:
  static const SMGValue GetNewValue();

  static const SMGValue& GetNullValue();
  static const SMGValue& GetInvalidValue();
  static const SMGValue& GetUnknownValue();

  long GetId() const;

  bool operator<(const SMGValue& other) const;
  bool operator==(const SMGValue& other) const;
  bool operator!=(const SMGValue& other) const;
};

}  // namespace smg
