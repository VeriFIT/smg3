/*
    Created by Viktor Malik on 24.2.2016.
*/

#pragma once

#include "graphs/CLangSMG.hh"

namespace smg {

class CLangSMGConsistencyVerifier {
  CLangSMGConsistencyVerifier();
  virtual ~CLangSMGConsistencyVerifier();

  static bool VerifyDisjunctHeapAndGlobal(const CLangSMG& smg);
  static bool VerifyDisjunctHeapAndStack(const CLangSMG& smg);
  static bool VerifyDisjunctGlobalAndStack(const CLangSMG& smg);
  static bool VerifyStackGlobalHeapUnion(const CLangSMG& smg);
  static bool VerifyNullObjectCLangProperties(const CLangSMG& smg);
  static bool VerifyGlobalNamespace(const CLangSMG& smg);
  static bool VerifyStackNamespaces(const CLangSMG& smg);

 public:
  static bool Verify(const CLangSMG& smg);
};

}  // namespace smg
