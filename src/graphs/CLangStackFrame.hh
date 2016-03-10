/*
    Created by Viktor Malik on 23.2.2016.
*/

#pragma once

#include <string>
#include <map>
#include <set>
#include "objects/SMGRegion.hh"

namespace smg {

class CLangStackFrame {
 private:
  std::string stack_function_;
  std::map<std::string, SMGRegionPtr> stack_variables_;

 public:
  explicit CLangStackFrame(const std::string& function);
  void AddStackVariable(const std::string name, const SMGRegionPtr& object);

  const std::string ToString() const;
  const SMGRegionPtr GetVariable(const std::string name) const;
  bool ContainsVariable(const std::string name) const;
  const std::map<std::string, SMGRegionPtr>& GetVariables() const;
  const std::set<SMGObjectPtr> GetAllObjects() const;
  const std::string GetFunctionDeclaration() const;
};

}  // namespace smg

