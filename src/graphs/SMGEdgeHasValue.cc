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
        throw IllegalArgumentException(
                "Call of overlapsWith() on Has-Value edges pair not originating from the same object");
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
//
//  public final boolean overlapsWith(final SMGEdgeHasValue other) {
//    if (getObject() != other.getObject()) {
//      String message = "Call of overlapsWith() on Has-Value edges pair not originating from the same object";
//      throw new IllegalArgumentException(message);
//    }
//
//    int otStart = other.getOffset();
//
//    int otEnd = otStart + other.getType().getSize();
//
//    return overlapsWith(otStart, otEnd);
//  }
//
//  public final boolean overlapsWith(final int pOtStart, final int pOtEnd) {
//
//    int myStart = offset;
//
//    int myEnd = myStart + type.getSize();
//
//    if (myStart < pOtStart) {
//      return (myEnd > pOtStart);
//
//    } else if (pOtStart < myStart) {
//      return (pOtEnd > myStart);
//    }
//
//    // Start offsets are equal, always overlap
//    return true;
//  }
//
//  public final boolean isCompatibleField(final SMGEdgeHasValue other) {
//    return type.equals(other.type) && (offset == other.offset);
//  }
//
//  public final boolean isCompatibleFieldOnSameObject(final SMGEdgeHasValue other) {
//    return type.getSize() == other.type.getSize() && (offset == other.offset) && getObject() == other.getObject();
//  }
//
//  @Override
//  @SuppressWarnings("checkstyle:avoidinlineconditionals")
//  public final int hashCode() {
//    final int prime = 31;
//    int result = super.hashCode();
//    result = prime * result + offset;
//    result = prime * result + ((type == null) ? 0 : type.hashCode());
//    return result;
//  }
//
//  @Override
//  public final boolean equals(final Object obj) {
//    if (this == obj) {
//      return true;
//    }
//    if (!super.equals(obj)) {
//      return false;
//    }
//    if (getClass() != obj.getClass()) {
//      return false;
//    }
//    SMGEdgeHasValue other = (SMGEdgeHasValue) obj;
//    if (offset != other.offset) {
//      return false;
//    }
//    if (type == null) {
//      if (other.type != null) {
//        return false;
//      }
//    } else if (!type.equals(other.type)) {
//      return false;
//    }
//    return true;
//  }
//}
