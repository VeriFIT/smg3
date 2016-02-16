#pragma once

#include <string>

namespace smg {

class SMGValue {
 private:
  static long id_counter_;
  long id_;
  explicit SMGValue(const long id);

 public:
  static const SMGValue GetNewValue();
  long GetId() const;
};

}  // namespace smg
