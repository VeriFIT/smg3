#pragma once

#include <string>

class SMGValue {
private:
    static long id_counter;
    long id;
    SMGValue(long id);
public:
    static const SMGValue getNewValue();
    long getId() const;
};

