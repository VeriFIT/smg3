#include <SMGEdgeHasValue.hh>
#include <IllegalArgumentException.hh>
#include <iostream>

SMGEdgeHasValue::SMGEdgeHasValue(const SMGCType& pType, const int pOffset, const SMGObject& pObject,
        const SMGValue& pValue) :
        SMGEdge(pValue, pObject), type(pType), offset(pOffset) {
}

const long SMGEdgeHasValue::getOffset() const {
    return offset;
}

const SMGCType& SMGEdgeHasValue::getType() const {
    return type;
}

const int SMGEdgeHasValue::getSizeInBytes() const {
    return type.getSize();
}

const bool SMGEdgeHasValue::isConsistentWith(const SMGEdgeHasValue& pOther) const {
    if (getObject().getId() == pOther.getObject().getId() && offset == pOther.offset && (&(type) == &(pOther.type))) {
        return getValue().getId() == pOther.getValue().getId();
    }

    return true;
}

const bool SMGEdgeHasValue::overlapsWith(const SMGEdgeHasValue& pOther) const {
    if (getObject().getId() != pOther.getObject().getId()) {
        std::string message = "Call of overlapsWith() on Has-Value edges pair not originating from the same object";
        throw IllegalArgumentException(message.c_str());
    }

    const int otStart = pOther.getOffset();
    const int otEnd = otStart + pOther.getSizeInBytes();

    return overlapsWith(otStart, otEnd);
}

const bool SMGEdgeHasValue::overlapsWith(const int pOtherStart, const int pOtherEnd) const {
    int myStart = offset;
    int myEnd = myStart + type.getSize();

    if (myStart < pOtherStart) {
        return (myEnd > pOtherStart);
    } else if (pOtherStart < myStart) {
        return (pOtherEnd > myStart);
    }

    // Start offsets are equal, always overlap
    return true;
}

const bool SMGEdgeHasValue::isCompatibleField(const SMGEdgeHasValue& pOther) const {
    return &type == &(pOther.type) && (offset == pOther.offset);
}

const bool SMGEdgeHasValue::isCompatibleFieldOnSameObject(const SMGEdgeHasValue& pOther) const {
    return isCompatibleField(pOther) && (getObject().getId() == pOther.getObject().getId());
}
