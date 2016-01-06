#include <SMGEdge.hh>
#include <SMGEdgePointsTo.hh>

SMGEdgePointsTo::SMGEdgePointsTo(const SMGValue& pValue, const SMGObject& pObject, int pOffset) :
        SMGEdge(pValue, pObject), offset(pOffset) {
}

const long SMGEdgePointsTo::getOffset() const {
    return offset;
}

bool SMGEdgePointsTo::isConsistentWith(const SMGEdgePointsTo &pOtherEdge) const {
    return SMGEdge::isConsistentWith(pOtherEdge) && offset == pOtherEdge.offset;
}
