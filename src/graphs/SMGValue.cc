#include "SMGValue.hh"
#include <string>

namespace smg {

long SMGValue::id_counter_ = 0;

const SMGValue SMGValue::GetNewValue() {
  SMGValue::id_counter_++;
  return SMGValue(SMGValue::id_counter_);
}

SMGValue::SMGValue(const long id) : id_(id) {}

long SMGValue::GetId() const { return id_; }

}  // namespace smg
