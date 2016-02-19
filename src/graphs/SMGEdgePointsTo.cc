#include "SMGEdgePointsTo.hh"

namespace smg {

SMGEdgePointsTo::SMGEdgePointsTo(const SMGValue& value,
                                 const SMGObjectPtr& object,
                                 const int offset)
    : SMGEdge(value, object), offset_(offset) { }

long SMGEdgePointsTo::GetOffset() const { return offset_; }

bool SMGEdgePointsTo::IsConsistentWith(const SMGEdgePointsTo& other) const {
  if (GetValue() != other.GetValue()) {
    if (offset_ == other.offset_ && GetObject() == other.GetObject()) {
      return false;
    }
  } else {
    if (offset_ != other.offset_ || GetObject() != other.GetObject()) {
      return false;
    }
  }

  return true;
}

}  // namespace smg
