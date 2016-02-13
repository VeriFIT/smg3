#pragma once

#include "SMG.hh"

class SMGConsistencyVerifier {
public:
    static bool verify(const SMG &pSmg);

private:
    static bool verifyNullObject(const SMG &pSmg);
    SMGConsistencyVerifier();
    virtual ~SMGConsistencyVerifier();
};
