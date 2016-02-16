#pragma once

#include <string>
#include <SMGValue.hh>
#include <SMGObject.hh>
#include <SMGEdge.hh>

class SMGEdgePointsTo: public SMGEdge {
private:
    long offset;
public:
    SMGEdgePointsTo(const SMGValue &pValue, const SMGObjectPtr &pObject, const int pOffset);
    long getOffset() const;
    bool isConsistentWith(const SMGEdgePointsTo &pOtherEdge) const;
};

typedef std::shared_ptr<const SMGEdgePointsTo> SMGEdgePointsToPtr;
