#pragma once

#include <SMGValue.hh>
#include <SMGObject.hh>

class SMGEdge {
    const SMGValue &value;
    const SMGObject &object;
protected:
    SMGEdge(const SMGValue &pValue, const SMGObject &pObject);
    bool isConsistentWith(const SMGEdge &pOtherEdge) const;
public:
    const SMGValue& getValue() const;
    const SMGObject& getObject() const;
};
