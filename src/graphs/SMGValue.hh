#pragma once

#include <string>

class SMGValue {
private:
    static long id_counter;
    const static SMGValue NULL_VALUE;
    const static SMGValue INVALID_VALUE;

    long id;

    SMGValue(long pId);
public:
    static const SMGValue getNewValue();
    static const SMGValue& getInvalidValue();
    static const SMGValue& getNullValue();
    long getId() const;
    bool operator<(const SMGValue &pOther) const;
    bool operator==(const SMGValue &pOther) const;
    bool operator!=(const SMGValue &pOther) const;
};
