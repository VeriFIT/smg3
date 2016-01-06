#include <string>
#include "SMGValue.hh"

long SMGValue::id_counter = 0;

const SMGValue SMGValue::getNewValue() {
    SMGValue::id_counter++;
    return SMGValue(SMGValue::id_counter);
}

SMGValue::SMGValue(const long pId) {
    this->id = pId;
}

long SMGValue::getId() const {
    return this->id;
}
