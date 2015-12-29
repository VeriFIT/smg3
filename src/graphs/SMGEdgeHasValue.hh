#pragma once

#include <string>
#include <SMGValue.hh>
#include <SMGObject.hh>
#include <SMGEdge.hh>
#include <SMGCType.hh>

class SMGEdgeHasValue: public SMGEdge {
private:
    const long offset;
    const SMGCType& type;
public:
    SMGEdgeHasValue(const SMGCType& pType, const int pOffset, const SMGObject& pObject, const SMGValue& pValue);
    const long getOffset() const;
    const SMGCType& getType() const;
    const int getSizeInBytes() const;
    const bool isConsistentWith(const SMGEdgeHasValue& pOther) const;
    const bool overlapsWith(const SMGEdgeHasValue& pOther) const;
    const bool overlapsWith(const int pOtherStart, const int pOtherEnd) const;
};
