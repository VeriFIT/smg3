#include <string>
#include "SMGValue.hh"

long SMGValue::id_counter = 0;
const SMGValue SMGValue::INVALID_VALUE = SMGValue(-1);
const SMGValue SMGValue::NULL_VALUE = SMGValue(0);

const SMGValue SMGValue::getNewValue() {
    SMGValue::id_counter++;
    return SMGValue(SMGValue::id_counter);
}

const SMGValue& SMGValue::getInvalidValue() {
    return SMGValue::INVALID_VALUE;
}

const SMGValue& SMGValue::getNullValue() {
    return SMGValue::NULL_VALUE;
}

SMGValue::SMGValue(const long pId) :
        id(pId) {
}

long SMGValue::getId() const {
    return this->id;
}

bool SMGValue::operator<(const SMGValue& pOther) const {
    return this->id < pOther.id;
}

bool SMGValue::operator==(const SMGValue& pOther) const {
    return this->id == pOther.id;
}

bool SMGValue::operator!=(const SMGValue& pOther) const {
    return !(*this == pOther);
}
