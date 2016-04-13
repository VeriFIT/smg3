/*
    Created by Viktor Malik on 23.2.2016.
*/

#pragma once

#include <map>
#include <set>
#include <string>
#include "objects/SMGRegion.hh"

namespace smg {

//TODO(anyone) Something like types/CFunctionDeclaration.java should be ported
class CLangStackFrame {
 private:
   /**
   * Function to which this stack frame belongs
   */
  std::string stack_function_;

  /**
  * A mapping from variable names to a set of SMG objects, representing
  * local variables.
  */
  std::map<std::string, SMGRegionPtr> stack_variables_;

  /**
  * An object to store function return value
  */
  SMGRegionPtr return_value_object_;  // TODO(anyone) finish porting this one

 public:
  // static char* RETVAL_LABEL = "___cpa_temp_result_var_";

  /**
  * Constructor. Creates an empty frame.
  *
  * @param pDeclaration Function for which the frame is created
  *
  * TODO: [PARAMETERS] Create objects for function parameters
  */
  explicit CLangStackFrame(const std::string& function);

  /**
  * Adds a SMG object pObj to a stack frame, representing variable pVariableName
  *
  * Throws {@link IllegalArgumentException} when some object is already
  * present with the name {@link pVariableName}
  *
  * @param pVariableName A name of the variable
  * @param pObject An object to put into the stack frame
  */
  void AddStackVariable(const std::string name, const SMGRegionPtr& object);

  /* ********************************************* */
  /* Non-modifying functions: getters and the like */
  /* ********************************************* */

  /**
  * @return String representation of the stack frame
  */
  const std::string ToString() const;

  /**
  * Getter for obtaining an object corresponding to a variable name
  *
  * Throws {@link NoSuchElementException} when passed a name not present
  *
  * @param pName Variable name
  * @return SMG object corresponding to pName in the frame
  */
  const SMGRegionPtr GetVariable(const std::string name) const;

  /**
  * @param pName Variable name
  * @return True if variable pName is present, false otherwise
  */
  bool ContainsVariable(const std::string name) const;

  /**
  * @return a mapping from variables name to SMGObjects
  */
  const std::map<std::string, SMGRegionPtr>& GetVariables() const;

  /**
  * @return a set of all objects: return value object, variables, parameters
  */
  const std::set<SMGObjectPtr> GetAllObjects() const;

  /**
  * @return Declaration of a function corresponding to the frame
  */
  const std::string GetFunctionDeclaration() const;

  /**
  * @return an {@link SMGObject} reserved for function return value
  */
  SMGRegionPtr GetReturnObject() const;  // TODO(anyone) finish porting this one
};

}  // namespace smg
