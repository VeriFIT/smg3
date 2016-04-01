#include "SMGValue.hh"
#include <string>

namespace smg {

long SMGValue::id_counter_ = 0;
const SMGValue SMGValue::NULL_VALUE = SMGValue(0);
const SMGValue SMGValue::INVALID_VALUE = SMGValue(-1);
const SMGValue SMGValue::UNKNOWN_VALUE = SMGValue(-2);  // TODO(anyone) what should be used here?

SMGValue::SMGValue(const long id) : id_(id) { }

const SMGValue SMGValue::GetNewValue() {
  SMGValue::id_counter_++;
  return SMGValue(SMGValue::id_counter_);
}

const SMGValue& SMGValue::GetNullValue() { return SMGValue::NULL_VALUE; }
const SMGValue& SMGValue::GetInvalidValue() { return SMGValue::INVALID_VALUE; }
const SMGValue& SMGValue::GetUnknownValue() { return SMGValue::UNKNOWN_VALUE; }

long SMGValue::GetId() const { return id_; }

bool SMGValue::operator<(const SMGValue& other) const { return id_ < other.id_; }
bool SMGValue::operator==(const SMGValue& other) const { return id_ == other.id_; }
bool SMGValue::operator!=(const SMGValue& other) const { return !(*this == other); }

}  // namespace smg
