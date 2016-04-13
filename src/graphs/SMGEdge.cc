#include "SMGEdge.hh"
#include "graphs/SMGValue.hh"
#include "objects/SMGObject.hh"

namespace smg {

SMGEdge::SMGEdge(const SMGValue& value, const SMGObjectPtr& object)
    : value_(value), object_(object) {}

const SMGValue& SMGEdge::GetValue() const { return value_; }

SMGObjectPtr SMGEdge::GetObject() const { return object_; }

bool SMGEdge::IsConsistentWith(const SMGEdge& other_edge) const {
  return other_edge.object_->GetId() != object_->GetId() &&
         other_edge.value_.GetId() != value_.GetId();
}

}  // namespace smg
