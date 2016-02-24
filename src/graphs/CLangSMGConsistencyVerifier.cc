/*
    Created by Viktor Malik on 24.2.2016.
*/

#include "CLangSMGConsistencyVerifier.hh"
#include <algorithm>

namespace smg {

CLangSMGConsistencyVerifier::CLangSMGConsistencyVerifier() { }

CLangSMGConsistencyVerifier::~CLangSMGConsistencyVerifier() { }

bool CLangSMGConsistencyVerifier::VerifyDisjunctHeapAndGlobal(const CLangSMG& smg) {
  std::set<SMGObjectPtr> globals;
  std::transform(smg.GetGlobalObjects().begin(),
                 smg.GetGlobalObjects().end(),
                 std::inserter(globals, globals.end()),
                 [](std::pair<std::string, SMGObjectPtr> obj) { return obj.second; });

  std::set<SMGObjectPtr> intersection;
  std::set_intersection(globals.begin(),
                        globals.end(),
                        smg.GetHeapObjects().begin(),
                        smg.GetHeapObjects().end(),
                        std::inserter(intersection, intersection.end()));
  return intersection.empty();
}

bool CLangSMGConsistencyVerifier::VerifyDisjunctHeapAndStack(const CLangSMG& smg) {
  std::set<SMGObjectPtr> stack;
  for (auto frame : smg.GetStackFrames()) {
    auto objects = frame.GetAllObjects();
    stack.insert(objects.begin(), objects.end());
  }

  std::set<SMGObjectPtr> intersection;
  std::set_intersection(stack.begin(),
                        stack.end(),
                        smg.GetHeapObjects().begin(),
                        smg.GetHeapObjects().end(),
                        std::inserter(intersection, intersection.end()));
  return intersection.empty();
}

bool CLangSMGConsistencyVerifier::VerifyDisjunctGlobalAndStack(const CLangSMG& smg) {
  std::set<SMGObjectPtr> globals;
  std::transform(smg.GetGlobalObjects().begin(),
                 smg.GetGlobalObjects().end(),
                 std::inserter(globals, globals.end()),
                 [](std::pair<std::string, SMGObjectPtr> obj) { return obj.second; });

  std::set<SMGObjectPtr> stack;
  for (auto frame : smg.GetStackFrames()) {
    auto objects = frame.GetAllObjects();
    stack.insert(objects.begin(), objects.end());
  }

  std::set<SMGObjectPtr> intersection;
  std::set_intersection(stack.begin(),
                        stack.end(),
                        globals.begin(),
                        globals.end(),
                        std::inserter(intersection, intersection.end()));
  return intersection.empty();
}

bool CLangSMGConsistencyVerifier::VerifyStackGlobalHeapUnion(const CLangSMG& smg) {
  std::set<SMGObjectPtr> object_union;

  object_union.insert(smg.GetHeapObjects().begin(), smg.GetHeapObjects().end());

  std::transform(smg.GetGlobalObjects().begin(),
                 smg.GetGlobalObjects().end(),
                 std::inserter(object_union, object_union.end()),
                 [](std::pair<std::string, SMGObjectPtr> obj) { return obj.second; });

  for (auto frame : smg.GetStackFrames()) {
    auto objects = frame.GetAllObjects();
    object_union.insert(objects.begin(), objects.end());
  }

  std::set<SMGObjectPtr> difference;
  std::set_symmetric_difference(object_union.begin(),
                                object_union.end(),
                                smg.GetObjects().begin(),
                                smg.GetObjects().end(),
                                std::inserter(difference, difference.end()));
  return difference.empty();
}

//  bool CLangSMGConsistencyVerifier::VerifyNullObjectCLangProperties(const CLangSMG& smg) {
//    return true;
//  }
//
//  bool CLangSMGConsistencyVerifier::VerifyGlobalNamespace(const CLangSMG& smg) {
//    return true;
//  }
//
//  bool CLangSMGConsistencyVerifier::VerifyStackNamespaces(const CLangSMG& smg) {
//    return true;
//  }

bool CLangSMGConsistencyVerifier::Verify(const CLangSMG& smg) {
  bool to_return = true;
  to_return = to_return && VerifyDisjunctHeapAndGlobal(smg);
  to_return = to_return && VerifyDisjunctHeapAndStack(smg);
  to_return = to_return && VerifyDisjunctGlobalAndStack(smg);
  to_return = to_return && VerifyStackGlobalHeapUnion(smg);
  return to_return;
}

}  // namespace smg
