#pragma once

#include <SMGValue.hh>
#include <SMGObject.hh>

class SMGEdge {
    const SMGValue value;
    const SMGObjectPtr object;
protected:
    SMGEdge(const SMGValue &pValue, const SMGObjectPtr &pObject);
    bool isConsistentWith(const SMGEdge &pOtherEdge) const;
public:
    const SMGValue& getValue() const;
    SMGObjectPtr getObject() const;
};
