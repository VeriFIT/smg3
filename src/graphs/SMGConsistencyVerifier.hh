#pragma once

#include "graphs/SMG.hh"

namespace smg {

class SMGConsistencyVerifier {
 public:
  static bool Verify(const SMG& smg);

 private:
  static bool VerifyNullObject(const SMG& smg);
  static bool VerifyEdgeConsistency(const SMG& smg);
  static bool VerifyGeneralEdgeConsistency(const SMG& smg, const SMGEdge& edge);
  static bool VerifyObjectConsistency(const SMG& smg);
  SMGConsistencyVerifier();
  virtual ~SMGConsistencyVerifier();
};

}  // namespace smg
