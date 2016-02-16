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
    SMGEdgeHasValue(const SMGCType &pType, const int pOffset, const SMGObjectPtr &pObject, const SMGValue &pValue);

    long getOffset() const;
    const SMGCType& getType() const;
    int getSizeInBytes() const;
    bool isConsistentWith(const SMGEdgeHasValue& pOther) const;
    bool overlapsWith(const SMGEdgeHasValue& pOther) const;
    bool overlapsWith(const long pOtherStart, const long pOtherEnd) const;
    bool isCompatibleField(const SMGEdgeHasValue& pOther) const;
    bool isCompatibleFieldOnSameObject(const SMGEdgeHasValue& pOther) const;
};

typedef std::shared_ptr<const SMGEdgeHasValue> SMGEdgeHasValuePtr;
