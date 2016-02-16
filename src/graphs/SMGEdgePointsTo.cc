#include <SMGEdge.hh>
#include <SMGEdgePointsTo.hh>

SMGEdgePointsTo::SMGEdgePointsTo(const SMGValue &pValue, const SMGObjectPtr &pObject, const int pOffset) :
        SMGEdge(pValue, pObject), offset(pOffset) {
}

long SMGEdgePointsTo::getOffset() const {
    return offset;
}

bool SMGEdgePointsTo::isConsistentWith(const SMGEdgePointsTo &pOtherEdge) const {
    return SMGEdge::isConsistentWith(pOtherEdge) && offset == pOtherEdge.offset;
}
