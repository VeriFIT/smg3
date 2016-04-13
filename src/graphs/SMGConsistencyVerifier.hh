#pragma once

#include "graphs/SMG.hh"

namespace smg {

class SMGConsistencyVerifier {
 public:
  static bool Verify(const SMG& smg);

 private:
   /**
   * A consistency checks related to the NULL object
   *
   * @param pLogger A logger to record results
   * @param pSmg A SMG to verify
   * @return True, if {@link pSmg} satisfies all consistency criteria
   */
  static bool VerifyNullObject(const SMG& smg);

  /**
  * Verifies that invalid regions do not have any Has-Value edges, as this
  * is forbidden in consistent SMGs
  *
  * @param pLogger A logger to record results
  * @param pSmg A SMG to verify
  * @return True, if {@link pSmg} satisfies all consistency criteria.
  */
  static bool VerifyInvalidRegionsHaveNoHVEdges(const SMG& smg);

  /**
  * Verifies that any fields (as designated by Has-Value edges) do not
  * exceed the boundary of the object.
  *
  * @param pLogger A logger to record results
  * @param pObject An object to verify
  * @param pSmg A SMG to verify
  * @return True, if {@link pObject} in {@link pSmg} satisfies all consistency criteria. False otherwise.
  */
  static bool CheckSingleFieldConsistency(const SMGObjectPtr& object, const SMG& smg);

  /**
  * Verify all objects satisfy the Field Consistency criteria
  * @param pLogger A logger to record results
  * @param pSmg A SMG to verify
  * @return True, if {@link pSmg} satisfies all consistency criteria. False otherwise.
  */
  static bool VerifyFieldConsistency(const SMG& smg);

  /**
  * Verify that the edges are consistent in the SMG
  *
  * @param pLogger A logger to record results
  * @param pSmg A SMG to verify
  * @return True, if all edges in {@link pEdges} satisfy consistency criteria. False otherwise.
  */
  static bool VerifyEdgeConsistency(const SMG& smg);
  
  //TODO(anyone) comment
  static bool VerifyGeneralEdgeConsistency(const SMG& smg, const SMGEdge& edge);

  //TODO(anyone) comment
  static bool VerifyObjectConsistency(const SMG& smg);

  //TODO(anyone) NEQ CONSISTENCY

  SMGConsistencyVerifier();
  virtual ~SMGConsistencyVerifier();
  
 public:
   //TODO(anyone) Where is VerifySmg ??? - NOT PORTED?

};

}  // namespace smg
