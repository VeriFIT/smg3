/*
    Created by Viktor Malik on 23.2.2016.
*/

#pragma once

#include <map>
#include <set>
#include <string>
#include "objects/SMGRegion.hh"

namespace smg {

class CLangStackFrame {
 private:
  std::string stack_function_;
  std::map<std::string, SMGRegionPtr> stack_variables_;
  SMGRegionPtr return_value_object_; //TODO(anyone) finish porting this one

 public:
  //static char* RETVAL_LABEL = "___cpa_temp_result_var_";
  explicit CLangStackFrame(const std::string& function);
  void AddStackVariable(const std::string name, const SMGRegionPtr& object);

  const std::string ToString() const;
  const SMGRegionPtr GetVariable(const std::string name) const;
  bool ContainsVariable(const std::string name) const;
  const std::map<std::string, SMGRegionPtr>& GetVariables() const;
  const std::set<SMGObjectPtr> GetAllObjects() const;
  const std::string GetFunctionDeclaration() const;
  SMGRegionPtr GetReturnObject() const; //TODO(anyone) finish porting this one
};

}  // namespace smg

