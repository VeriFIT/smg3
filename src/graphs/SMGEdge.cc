#include <SMGEdge.hh>
#include <SMGValue.hh>
#include <SMGObject.hh>

SMGEdge::SMGEdge(const SMGValue& pValue, const SMGObject& pObject) :
        value(pValue), object(pObject) {
}

const SMGValue& SMGEdge::getValue() const {
    return value;
}

const SMGObject& SMGEdge::getObject() const {
    return object;
}

bool SMGEdge::isConsistentWith(const SMGEdge &pOtherEdge) const {
    return pOtherEdge.object.getId() == object.getId() && pOtherEdge.value.getId() == value.getId();
}
