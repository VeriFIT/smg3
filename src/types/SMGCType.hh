#pragma once

class SMGCType {
private:
    const int size;
    static const int SIZE_INT = 4;
    static const int SIZE_POINTER = 8;
public:
    const static SMGCType createTypeWithSize(const int pSize);
    const static SMGCType getIntType();
    const static SMGCType getPointerType();

    SMGCType(const int pSize);
    const int getSize() const;
};
