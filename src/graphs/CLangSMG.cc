/*
    Created by Viktor Malik on 24.2.2016.
*/

#include "CLangSMG.hh"
#include <algorithm>
#include <iterator>
#include <list>
#include "exceptions/IllegalArgumentException.hh"
#include "exceptions/UnsupportedOperationException.hh"

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

void CLangSMG::RemoveHeapObject(const SMGRegionPtr& object) {
  if (IsHeapObject(object)) {
    heap_objects_.erase(object);
    RemoveObjectAndEdges(object);
  } else {
    throw IllegalArgumentException("Cannot directly remove non-heap objects");
  }
}

SMGRegionPtr CLangSMG::AddGlobalVariable(const SMGCType& type, const std::string var_name) {
  int size = type.GetSize();
  SMGRegionPtr new_object = std::make_shared<SMGRegion>(size, var_name);
  AddGlobalObject(new_object);
  return new_object;
}

SMGRegionPtr CLangSMG::AddLocalVariable(const SMGCType& type, const std::string var_name) {
  int size = type.GetSize();
  SMGRegionPtr new_object = std::make_shared<SMGRegion>(size, var_name);
  AddStackObject(new_object);
  return new_object;
}

void CLangSMG::free(const int offset, const SMGRegionPtr& region) {
  if (!IsHeapObject(region)) {
    // You may not free any objects not on the heap.
    // setInvalidFree();
    return;
  }

  if (!(offset == 0)) {
    // you may not invoke free on any address that you
    // didn't get through a malloc invocation.
    // setInvalidFree();
    return;
  }

  if (!IsObjectValid(region)) {
    // you may not invoke free multiple times on
    // the same object
    // setInvalidFree();
    return;
  }

  //TODO: sub-optimal, could be replaced with std::set::erase and single iteration approach
  SetValidity(region, false);
  SMGEdgeHasValueFilter filter = SMGEdgeHasValueFilter::ObjectFilter(region);

  for (auto edge : GetHVEdges(filter)) {
    RemoveHasValueEdge(edge);
  }
}

void CLangSMG::SetMemoryLeak() { has_leaks_ = true; }

bool CLangSMG::HasMemoryLeaks() const { return has_leaks_; }

const std::deque<CLangStackFrame>& CLangSMG::GetStackFrames() const { return stack_objects_; }

const std::set<SMGObjectPtr>& CLangSMG::GetHeapObjects() const { return heap_objects_; }

const std::set<SMGObjectPtr> CLangSMG::GetGlobalObjects() const {
  std::set<SMGObjectPtr> globals;
  std::transform(global_objects_.begin(),
                 global_objects_.end(),
                 std::inserter(globals, globals.end()),
                 [](std::pair<std::string, SMGObjectPtr> obj) { return obj.second; });
  return globals;
}

const std::map<std::string, SMGRegionPtr>& CLangSMG::GetGlobalVariables() const {
  return global_objects_;
}

SMGRegionPtr CLangSMG::GetObjectForVisibleVariable(const std::string variable_name) const {
  // Look in the local frame
  if (stack_objects_.size() != 0) {
    if (stack_objects_.front().ContainsVariable(variable_name)) {
      return stack_objects_.front().GetVariable(variable_name);
    }
  }

  // Look in the global scope
  if (global_objects_.find(variable_name) != global_objects_.end()) {
    return global_objects_.at(variable_name);
  }

  std::string msg = "No object for variable name: " + variable_name;
  throw UnsupportedOperationException(msg.c_str());
}
bool CLangSMG::HasLocalVariable(const std::string variable_name) const {
  return (stack_objects_.size() > 0) && stack_objects_.front().ContainsVariable(variable_name);
}

bool CLangSMG::IsHeapObject(const SMGObjectPtr& object) const {
  return heap_objects_.find(object) != heap_objects_.end();
}

bool CLangSMG::IsGlobalObject(const SMGObjectPtr& object) const {
  if (object->IsAbstract()) {
    return false;
  }
  auto globals = GetGlobalObjects();
  return globals.find(object) != globals.end();
}

bool CLangSMG::ContainsValue(const SMGValue& value) const {
  return GetValues().find(value) != GetValues().end();
}

/**
* Get the symbolic value, that represents the address
* pointing to the given memory with the given offset, if it exists.
*
* @param memory get address belonging to this memory.
* @param offset get address with this offset relative to the beginning of the memory.
* @return Address of the given field, or null, if such an address does not yet exist in the SMG.
*/
const SMGValue& CLangSMG::GetAddress(const SMGObjectPtr& memory, const long offset) const {  
  for (auto edge : GetPTEdges()) {
    if (*edge.second->GetObject() == *memory && edge.second->GetOffset() == offset)
      return edge.second->GetValue();
  }
  //? or InvalidValue? what is the semantics - should be commented in SMGValue
  return SMGValue::GetNullValue(); 
}

}  // namespace smg
