/*
    Created by Viktor Malik on 24.2.2016.
*/

#pragma once

#include <map>
#include <set>
#include <deque>
#include <string>
#include "graphs/CLangStackFrame.hh"
#include "graphs/SMG.hh"
#include "objects/SMGObject.hh"
#include "objects/SMGRegion.hh"

namespace smg {

class CLangSMG : public SMG {
 private:
  std::deque<CLangStackFrame> stack_objects_;
  std::set<SMGObjectPtr> heap_objects_;
  std::map<std::string, SMGRegionPtr> global_objects_;

  bool has_leaks_;
  static bool perform_checks_;

 public:
  static void SetPerformChecks(const bool setting);
  static bool PerformChecks();
  CLangSMG();

  void SetMemoryLeak();
  bool HasMemoryLeaks() const;

  void AddHeapObject(const SMGObjectPtr& object);
  void AddGlobalObject(const SMGRegionPtr& object);
  void AddStackObject(const SMGRegionPtr& object);
  void AddStackFrame(const std::string function_declaration);
  void DropStackFrame();

  const std::deque<CLangStackFrame>& GetStackFrames() const;
  const std::set<SMGObjectPtr>& GetHeapObjects() const;
  const std::set<SMGObjectPtr> GetGlobalObjects() const;
  const std::map<std::string, SMGRegionPtr>& GetGlobalVariables() const;
};

}  // namespace smg


