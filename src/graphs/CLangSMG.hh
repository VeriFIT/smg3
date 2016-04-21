/*
    Created by Viktor Malik on 24.2.2016.
*/

#pragma once

#include <deque>
#include <map>
#include <set>
#include <string>
#include "graphs/CLangStackFrame.hh"
#include "graphs/SMG.hh"
#include "objects/SMGObject.hh"
#include "objects/SMGRegion.hh"

namespace smg {

/**
* Extending SMG with notions specific for programs in C language:
*  - separation of global, heap and stack objects
*  - null object and value
*/
class CLangSMG : public SMG {
 private:
  /**
  * A container for object found on the stack:
  *  - local variables
  *  - parameters
  *
  * TODO: [STACK-FRAME-STRUCTURE] Perhaps it could be wrapped in a class?
  */
  std::deque<CLangStackFrame> stack_objects_;

  /**
  * A container for objects allocated on heap
  */

  std::set<SMGObjectPtr> heap_objects_;
  /**
  * A container for global objects
  */
  std::map<std::string, SMGRegionPtr> global_objects_;

  /**
  * A flag signifying the edge leading to this state caused memory to be leaked
  * TODO: Seems pretty arbitrary: perhaps we should have a more general
  * solution, like a container with (type, message) error witness kind of
  * thing?
  */
  bool has_leaks_;

  /**
  * A flag setting if the class should perform additional consistency checks.
  * It should be useful only during debugging, when is should find bad external
  * calls closer to their origin. We probably do not want t run the checks in
  * the production build.
  */
  static bool perform_checks_;

 public:
  static void SetPerformChecks(const bool setting);
  static bool PerformChecks();

  /**
  * Constructor.
  *
  * Keeps consistency: yes
  *
  * Newly constructed CLangSMG contains a single nullObject with an address
  * pointing to it, and is empty otherwise.
  */
  CLangSMG();

  /**
  * Sets a flag indicating this SMG is a successor over the edge causing a
  * memory leak.
  *
  * Keeps consistency: yes
  */
  void SetMemoryLeak();

  /**
   * Constant.
   *
   * @return True if the SMG is a successor over the edge causing some memory
   * to be leaked. Returns false otherwise.
   */
  bool HasMemoryLeaks() const;

  /**
  * Add a object to the heap.
  *
  * Keeps consistency: no
  *
  * With checks: throws {@link IllegalArgumentException} when asked to add an
  * object already present.
  *
  * @param object
  *          Object to add.
  */
  void AddHeapObject(const SMGObjectPtr& object);

  /**
  * Add a global object to the SMG
  *
  * Keeps consistency: no
  *
  * With checks: throws {@link IllegalArgumentException} when asked to add
  * an object already present, or an global object with a label identifying
  * different object
  * @param pObject object to add
  */
  void AddGlobalObject(const SMGRegionPtr& object);

  /**
  * Adds an object to the current stack frame
  *
  * Keeps consistency: no
  *
  * @param pObject Object to add
  *
  * TODO: [SCOPES] Scope visibility vs. stack frame issues: handle cases where a variable is visible
  * but is is allowed to override (inner blocks)
  * TODO: Consistency check (allow): different objects with same label inside a frame, but
  * in different block
  * TODO: Test for this consistency check
  *
  * TODO: Shall we need an extension for putting objects to upper frames?
  */
  void AddStackObject(const SMGRegionPtr& object);

  /**
  * Add a new stack frame for the passed function.
  *
  * Keeps consistency: yes
  *
  * @param pFunctionDeclaration A function for which to create a new stack frame
  */
  void AddStackFrame(const std::string function_declaration);

  /**
  * Remove a top stack frame from the SMG, along with all objects in it, and
  * any edges leading from/to it.
  *
  * TODO: A testcase with (invalid) passing of an address of a dropped frame object
  * outside, and working with them. For that, we should probably keep those as invalid, so
  * we can spot such bug.
  *
  * Keeps consistency: yes
  */
  void DropStackFrame();

  void RemoveHeapObject(const SMGRegionPtr& object);

  /**
  * Makes SMGState create a new object and put it into the global namespace
  *
  * Keeps consistency: yes
  *
  * @param type Type of the new object
  * @param var_name Name of the global variable
  * @return Newly created object
  *
  * @throws SMGInconsistentException when resulting SMGState is inconsistent
  * and the checks are enabled
  */
  SMGRegionPtr AddGlobalVariable(const SMGCType& type, const std::string var_name);

  /**
  * Makes SMGState create a new object and put it into the current stack
  * frame.
  *
  * Keeps consistency: yes
  *
  * @param type Type of the new object
  * @param var_name Name of the local variable
  * @return Newly created object
  *
  * @throws SMGInconsistentException when resulting SMGState is inconsistent
  * and the checks are enabled
  */
  SMGRegionPtr AddLocalVariable(const SMGCType& type, const std::string var_name);

  /**
   * This method simulates a free invocation. It checks,
   * whether the call is valid, and invalidates the
   * Memory the given address points to.
   * The address (address, offset, smgObject) is the argument
   * of the free invocation. It does not need to be part of the SMG.
   *
   * @param pAddress The symbolic Value of the address.  //-----------------NOT USED IN ORIGINAL???
   * @param offset The offset of the address relative to the beginning of smgObject.
   * @param region The memory the given Address belongs to.
   * @throws SMGInconsistentException
   */
  void free(const int offset, const SMGRegionPtr& region);

  /**
  * Returns the stack of frames containing objects. Constant.
  *
  * @return Stack of frames
  */
  const std::deque<CLangStackFrame>& GetStackFrames() const;

  /**
  * Constant.
  *
  * @return Unmodifiable view of the set of the heap objects
  */
  const std::set<SMGObjectPtr>& GetHeapObjects() const;

  /**
  * Constant.
  *
  * @return Unmodifiable map from variable names to global objects.
  */
  const std::set<SMGObjectPtr> GetGlobalObjects() const;
  const std::map<std::string, SMGRegionPtr>& GetGlobalVariables() const;
  SMGRegionPtr GetObjectForVisibleVariable(const std::string variable_name) const;

  bool HasLocalVariable(const std::string variable_name) const;

  /**
  * Constant.
  *
  * Checks whether given object is on the heap.
  *
  * @param object SMGObject to be checked.
  * @return True, if the given object is referenced in the set of heap objects, false otherwise.
  *
  */
  bool IsHeapObject(const SMGObjectPtr& object) const;
  bool IsGlobalObject(const SMGObjectPtr& object) const;
  bool ContainsValue(const SMGValue& value) const;

  /**
  * Get the symbolic value, that represents the address
  * pointing to the given memory with the given offset, if it exists.
  *
  * @param memory get address belonging to this memory.
  * @param offset get address with this offset relative to the beginning of the memory.
  * @return Address of the given field, or null, if such an address does not yet exist in the SMG.
  */
  const SMGValue& GetAddress(const SMGObjectPtr& memory, long offset) const;
  
  /**
  * Read Value in field (object, type) of an Object.
  *
  * This method does not modify the state being read,
  * and is therefore safe to call outside of a
  * transfer relation context.
  *
  * @param pObject SMGObject representing the memory the field belongs to.
  * @param pOffset offset of field being read.ReadValue
  * @param pType type of field
  * @return A Symbolic value, if found, otherwise null.
  * @throws SMGInconsistentException
  */
  const SMGValue& ReadValue(const SMGObjectPtr& object, long offset, const SMGCType& type) const;
  const SMGValue& ReadValue(const SMGEdgePointsTo& ptEdge, const SMGCType & type) const;

  //-----------------------------------------------------------------------------------------------
  //Added for NextGen v0.1

  const SMGEdgePointsTo& ClangGetTargetPtEdge(const SMGRegionPtr& tempRegion) const;
  const SMGValue& ClangReadValue(const SMGRegionPtr& tempRegion, const SMGCType& type) const;

  //-----------------------------------------------------------------------------------------------
  //NOT PORTED

  /**
  * Getter for obtaining a string representation of the CLangSMG. Constant.
  *
  * @return String representation of the CLangSMG
  */
  //@Override
  //  public String toString() {....

  /**
  * Prune the SMG: remove all unreachable objects (heap ones: global and stack
  * are always reachable) and values.
  *
  * TODO: Too large. Refactor into fewer pieces
  *
  * Keeps consistency: yes
  */
  //@Override
  //  public void pruneUnreachable() {...

  /**
  * Returns an SMGObject tied to the variable name. The name must be visible in
  * the current scope: it needs to be visible either in the current frame, or it
  * is a global variable. Constant.
  *
  * @param pVariableName A name of the variable
  * @return An object tied to the name, if such exists in the visible scope.
  *
  * TODO: [SCOPES] Test for getting visible local object hiding other local object
  * @throws SMGInconsistentException
  */
  //@Override
  //  public SMGRegion getObjectForVisibleVariable(final String pVariableName) { ...

  /**
  * Constant.
  *
  * @return a {@link SMGObject} for current function return value
  */
  //@Override
  //  public SMGRegion getStackReturnObject(final int pUp) {
  //  return stackObjects.peek().getReturnObject();
  //}

  //@Override
  //  public String getFunctionName(final SMGRegion pObject) {
  //  for (CLangStackFrame cLangStack : stackObjects) {
  //    if (cLangStack.getAllObjects().contains(pObject)) {
  //      return cLangStack.getFunctionDeclaration().getName();
  //    }
  //  }

  //  throw new IllegalArgumentException("No function name for non-stack object");
  //}

  //@Override
  //  public void mergeValues(final int v1, final int v2) {
  //  super.mergeValues(v1, v2);
  //}

  /**
  * Determine, whether the two given symbolic values are not equal.
  * If this method does not return true, the relation of these
  * symbolic values is unknown.
  *
  * @param value1 first symbolic value to be checked
  * @param value2 second symbolic value to be checked
  * @return true, if the symbolic values are known to be not equal, false, if it is unknown.
  * @throws SMGInconsistentException
  */
  //@Override
  //public boolean isUnequal(final int value1, final int value2) {...

  /**
  * Get the symbolic value, that represents the address
  * pointing to the given memory with the given offset, if it exists.
  *
  * @param memory
  *          get address belonging to this memory.
  * @param offset
  *          get address with this offset relative to the beginning of the
  *          memory.
  * @return Address of the given field, or null, if such an address does not
  *         yet exist in the SMG.
  */
  //@Override
  //public Integer getAddress(final SMGObject pMemory, final Integer pOffset) { ...

  //@Override
  //public boolean isIdenticalTo(final ReadableSMG pOther) {
  //  if (!(pOther instanceof CLangSMG)) {
  //    throw new IllegalArgumentException("Cannot compare CLangSMG to non-CLangSMG");
  //  }

  //  CLangSMG other = (CLangSMG)pOther;

  //  if (!(super.isIdenticalTo(other))) {
  //    return false;
  //  }

  //  boolean stackIdentical = Arrays.equals(stackObjects.toArray(), other.stackObjects.toArray());
  //  boolean heapIdentical = heapObjects.equals(other.heapObjects);
  //  boolean globalIdentical = globalObjects.equals(other.globalObjects);

  //  return stackIdentical && heapIdentical && globalIdentical;
  //}

  //-----------------------------------------------------------------------------------------------
  //ALSO NOT PORTED (WILL EVER BE?)

  //SMGSymbolicValue readValue(final SMGObject pObject, final int pOffset, final CType pType)
};

}  // namespace smg
