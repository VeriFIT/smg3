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
    long getOffset() const;
    const SMGCType& getType() const;
    int getSizeInBytes() const;
    bool isConsistentWith(const SMGEdgeHasValue& pOther) const;
    bool overlapsWith(const SMGEdgeHasValue& pOther) const;
    bool overlapsWith(const int pOtherStart, const int pOtherEnd) const;
    bool isCompatibleField(const SMGEdgeHasValue& pOther) const;
    bool isCompatibleFieldOnSameObject(const SMGEdgeHasValue& pOther) const;
};
