/*
    Created by Viktor Malik on 23.2.2016.
*/

#include "CLangStackFrame.hh"
#include <sstream>
#include "exceptions/IllegalArgumentException.hh"
#include "exceptions/NoSuchElementException.hh"

namespace smg {

void CLangStackFrame::AddStackVariable(const std::string name, const SMGRegionPtr& object) {
  if (stack_variables_.find(name) != stack_variables_.end()) {
    const std::string msg = "Stack frame already contains variable " + name;
    throw IllegalArgumentException(msg.c_str());
  }

  stack_variables_[name] = object;
}

const std::string CLangStackFrame::ToString() const {
  std::ostringstream to_return("<");
  for (auto var : stack_variables_) {
    to_return << " " << var.first << "=" << var.second->ToString();
  }
  to_return << " >";
  return to_return.str();
}

const SMGRegionPtr CLangStackFrame::GetVariable(const std::string name) const {
  try {
    SMGRegionPtr to_return = stack_variables_.at(name);
    return to_return;
  } catch (std::out_of_range e) {
    std::string msg = "No variable with name " + name + " in stack frame";
    throw NoSuchElementException(msg.c_str());
  }
}

bool CLangStackFrame::ContainsVariable(const std::string name) const {
  return (stack_variables_.find(name) != stack_variables_.end());
}

const std::map<std::string, SMGRegionPtr>& CLangStackFrame::GetVariables() const {
  return stack_variables_;
}

const std::set<SMGObjectPtr> CLangStackFrame::GetAllObjects() const {
  std::set<SMGObjectPtr> to_return;
  for (auto var : stack_variables_) {
    to_return.insert(var.second);
  }
  return to_return;
//  if (returnValueObject != null) {
//    retset.add(returnValueObject);
//  }
}

}  // namespace smg
