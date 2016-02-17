#pragma once

#include "graphs/SMG.hh"

namespace smg {

class SMGConsistencyVerifier {
 public:
  static bool Verify(const SMG& smg);

 private:
  static bool VerifyNullObject(const SMG& smg);
  SMGConsistencyVerifier();
  virtual ~SMGConsistencyVerifier();
};

}  // namespace smg
