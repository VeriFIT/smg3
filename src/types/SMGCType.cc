#include <SMGCType.hh>

const SMGCType SMGCType::createTypeWithSize(const int pSize) {
    return SMGCType(pSize);
}
const SMGCType SMGCType::getIntType() {
    return createTypeWithSize(SMGCType::SIZE_INT);
}

const SMGCType SMGCType::getPointerType() {
    return createTypeWithSize(SMGCType::SIZE_POINTER);
}

SMGCType::SMGCType(const int pSize) :
        size(pSize) {
}

const int SMGCType::getSize() const {
    return size;
}
