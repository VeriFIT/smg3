/*
    Created by Viktor Malik on 24.2.2016.
*/

#include <exceptions/IllegalArgumentException.hh>
#include "CLangSMG.hh"

namespace smg {

bool CLangSMG::perform_checks_ = false;

void CLangSMG::SetPerformChecks(const bool setting) { perform_checks_ = setting; }

bool CLangSMG::PerformChecks() { return perform_checks_; }

CLangSMG::CLangSMG() : has_leaks_(false) { heap_objects_.insert(GetNullObject()); }

void CLangSMG::AddHeapObject(const SMGObjectPtr& object) {
  if (perform_checks_ && heap_objects_.find(object) != heap_objects_.end()) {
    std::string msg = "Heap object already in the SMG: [" + object->GetLabel() + "]";
    throw IllegalArgumentException(msg.c_str());
  }
  heap_objects_.insert(object);
  AddObject(object);
}

void CLangSMG::AddGlobalObject(const SMGRegionPtr& object) {
  if (perform_checks_ && global_objects_.find(object->GetLabel()) != global_objects_.end()) {
    std::string msg = "Global object with label [" + object->GetLabel() + "] already in the SMG";
    throw IllegalArgumentException(msg.c_str());
  }

  global_objects_[object->GetLabel()] = object;
  AddObject(object);
}

void CLangSMG::AddStackObject(const SMGRegionPtr& object) {
  AddObject(object);
  stack_objects_.front().AddStackVariable(object->GetLabel(), object);
}

void CLangSMG::AddStackFrame(const std::string function_declaration) {
  CLangStackFrame new_frame = CLangStackFrame(function_declaration);
  stack_objects_.emplace_front(new_frame);
}

void CLangSMG::DropStackFrame() {
  CLangStackFrame frame = stack_objects_.front();
  stack_objects_.pop_front();

  for (auto object : frame.GetAllObjects()) {
    RemoveObjectAndEdges(object);
  }

  if (perform_checks_) {
    // Consistency verifier
  }
}

void CLangSMG::SetMemoryLeak() { has_leaks_ = true; }

bool CLangSMG::HasMemoryLeaks() const { return has_leaks_; }

const std::deque<CLangStackFrame>& CLangSMG::GetStackFrames() const { return stack_objects_; }

const std::set<SMGObjectPtr>& CLangSMG::GetHeapObjects() const { return heap_objects_; }

const std::map<std::string, SMGRegionPtr>& CLangSMG::GetGlobalObjects() const {
  return global_objects_;
}

}  // namespace smg
