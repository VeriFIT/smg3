/*
    Created by Viktor Malik on 24.2.2016.
*/

#pragma once

#include "graphs/CLangSMG.hh"

namespace smg {

class CLangSMGConsistencyVerifier {
  CLangSMGConsistencyVerifier();
  virtual ~CLangSMGConsistencyVerifier();


  /**
  * Verifies that heap and global object sets are disjunct
  *
  * @param pLogger Logger to log the message
  * @param pSmg SMG to check
  * @return True if {@link pSmg} is consistent w.r.t. this criteria. False otherwise.
  */
  static bool VerifyDisjunctHeapAndGlobal(const CLangSMG& smg);

  /**
  * Verifies that heap and stack object sets are disjunct
  *
  * @param pLogger Logger to log the message
  * @param pSmg SMG to check
  * @return True if {@link pSmg} is consistent w.r.t. this criteria. False otherwise.
  */
  static bool VerifyDisjunctHeapAndStack(const CLangSMG& smg);

  /**
  * Verifies that global and stack object sets are disjunct
  *
  * @param pLogger Logger to log the message
  * @param pSmg SMG to check
  * @return True if {@link pSmg} is consistent w.r.t. this criteria. False otherwise.
  */
  static bool VerifyDisjunctGlobalAndStack(const CLangSMG& smg);

  /**
  * Verifies that heap, global and stack union is equal to the set of all objects
  *
  * @param pLogger Logger to log the message
  * @param pSmg SMG to check
  * @return True if {@link pSmg} is consistent w.r.t. this criteria. False otherwise.
  */
  static bool VerifyStackGlobalHeapUnion(const CLangSMG& smg);

  /**
  * Verifies several NULL object-related properties
  * @param pLogger Logger to log the message
  * @param pSmg SMG to check
  *
  * @return True if {@link pSmg} is consistent w.r.t. this criteria. False otherwise.
  */
  static bool VerifyNullObjectCLangProperties(const CLangSMG& smg);

  /**
  * Verify the global scope is consistent: each record points to an
  * appropriately labeled object
  *
  * @param pLogger Logger to log the message
  * @param pSmg SMG to check
  * @return True if {@link pSmg} is consistent w.r.t. this criteria. False otherwise.
  */
  static bool VerifyGlobalNamespace(const CLangSMG& smg);

  /**
  * Verify the stack name space: each record points to an appropriately
  * labeled object
  *
  * @param pLogger Logger to log the message
  * @param pSmg
  * @return True if {@link pSmg} is consistent w.r.t. this criteria. False otherwise.
  */
  static bool VerifyStackNamespaces(const CLangSMG& smg);

 public:
   /**
   * Verify all the consistency properties related to CLangSMG
   *
   * @param pLogger Logger to log results
   * @param pSmg SMG to check
   * @return True if {@link pSmg} is consistent w.r.t. this criteria. False otherwise.
   */
  static bool Verify(const CLangSMG& smg);
};

}  // namespace smg
