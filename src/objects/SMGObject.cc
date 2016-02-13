#include "SMGObject.hh"

long SMGObject::id_counter = 1;
const SMGObjectPtr SMGNullObject::NULL_OBJECT = SMGObjectPtr(new SMGNullObject());

const SMGObjectPtr SMGNullObject::getNullObject() {
    return SMGNullObject::NULL_OBJECT;
}

SMGObject::SMGObject(const int pSize, const std::string &pLabel) :
        size(pSize), label(pLabel) {
    id = SMGObject::id_counter++;
}

SMGObject::~SMGObject() {
}

// ReSharper disable once CppMemberFunctionMayBeStatic
std::string SMGObject::getClassName() const {
    return std::string("SMGObject");
}

const std::string& SMGObject::getLabel() const {
    return label;
}

int SMGObject::getSize() const {
    return size;
}

bool SMGObject::notNull() const {
    return true;
}

bool SMGNullObject::notNull() const {
    return false;
}

long SMGObject::getId() const {
    return id;
}

SMGNullObject::SMGNullObject() :
        SMGObject(0, "NULL") {
}

bool SMGNullObject::isAbstract() const {
    return false;
}

void SMGNullObject::accept(SMGObjectVisitor &pVisitor __attribute__((unused))) const {
}

bool SMGNullObject::isMoreGeneral(const SMGObject &pOther __attribute__((unused))) const {
    return false;
}
