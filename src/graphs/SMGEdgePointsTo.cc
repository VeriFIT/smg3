#include "SMGEdgePointsTo.hh"
#include "graphs/SMGEdge.hh"

namespace smg {

SMGEdgePointsTo::SMGEdgePointsTo(const SMGValue& value,
                                 const SMGObjectPtr& object,
                                 const int offset)
    : SMGEdge(value, object), offset_(offset) {}

long SMGEdgePointsTo::GetOffset() const { return offset_; }

bool SMGEdgePointsTo::IsConsistentWith(const SMGEdgePointsTo& other_edge) const {
  return SMGEdge::IsConsistentWith(other_edge) && offset_ == other_edge.offset_;
}

}  // namespace smg
