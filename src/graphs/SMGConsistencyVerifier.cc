/*
 * SMGConsistencyVerifier.cc
 *
 *  Created on: Jan 28, 2016
 *      Author: pmuller
 */

#include "SMGConsistencyVerifier.hh"

namespace smg {

bool SMGConsistencyVerifier::VerifyNullObject(const SMG& smg) {
  SMGValue null_value = SMGValue::GetInvalidValue();
  for (SMGValue value : smg.GetValues()) {
    if (smg.GetObjectPointedBy(value) == smg.GetNullObject()) {
      null_value = value;
      break;
    }
  }

  if (null_value == SMGValue::GetInvalidValue()) {
    return false;
  }

  if (smg.GetObjectPointedBy(smg.GetNullValue()) != smg.GetNullObject()) {
    return false;
  }

  if (smg.GetNullValue() != null_value) {
    return false;
  }

  //
  //    // Verify that NULL object has no value
  //    SMGEdgeHasValueFilter filter = SMGEdgeHasValueFilter.objectFilter(pSmg.getNullObject());
  //
  //    if (pSmg.getHVEdges(filter).iterator().hasNext()) {
  //      // pLogger.log(Level.SEVERE, "SMG inconsistent: null object has some value");
  //      return false;
  //    }
  //
  //    // Verify that the NULL object is invalid
  //    if (pSmg.isObjectValid(pSmg.getNullObject())) {
  ////      pLogger.log(Level.SEVERE, "SMG inconsistent: null object is not invalid");
  //      return false;
  //    }

  if (smg.GetNullObject()->GetSize() != 0) {
    return false;
  }

  return true;
}

bool SMGConsistencyVerifier::Verify(const SMG& smg) {
  bool to_return = true;
  to_return = to_return && VerifyNullObject(smg);
  return to_return;
  //      toReturn = toReturn && verifySMGProperty(
  //          verifyNullObject(pSmg), "null object invariants hold");
  //      toReturn = toReturn && verifySMGProperty(
  //          verifyInvalidRegionsHaveNoHVEdges(pSmg), "invalid regions have no outgoing edges");
  //      toReturn = toReturn && verifySMGProperty(
  //          verifyFieldConsistency(pSmg), "field consistency");
  //      toReturn = toReturn && verifySMGProperty(
  //          verifyEdgeConsistency(pSmg, pSmg.getHVEdges()), "Has Value edge consistency");
  //      toReturn = toReturn && verifySMGProperty(
  //          verifyEdgeConsistency(pSmg, pSmg.getPTEdges()), "Points To edge consistency");
  //      toReturn = toReturn && verifySMGProperty(
  //          verifyObjectConsistency(pSmg), "Validity consistency");
  //
  //  //    pLogger.log(Level.FINEST, "Ending consistency check of a SMG");
  //
  //      return toReturn;
  //    }
}

SMGConsistencyVerifier::SMGConsistencyVerifier() {}

SMGConsistencyVerifier::~SMGConsistencyVerifier() {}
//
//  /**
//   * A consistency checks related to the NULL object
//   *
//   * @param pLogger A logger to record results
//   * @param pSmg A SMG to verify
//   * @return True, if {@link pSmg} satisfies all consistency criteria
//   */
//  private static boolean verifyNullObject(final SMG pSmg) {

//  }
//
//  /**
//   * Verifies that invalid regions do not have any Has-Value edges, as this
//   * is forbidden in consistent SMGs
//   *
//   * @param pLogger A logger to record results
//   * @param pSmg A SMG to verify
//   * @return True, if {@link pSmg} satisfies all consistency criteria.
//   */
//  private static boolean verifyInvalidRegionsHaveNoHVEdges(final SMG pSmg) {
//    for (SMGObject obj : pSmg.getObjects()) {
//      if (pSmg.isObjectValid(obj)) {
//        continue;
//      }
//      // Verify that the HasValue edge set for this invalid object is empty
//      SMGEdgeHasValueFilter filter = SMGEdgeHasValueFilter.objectFilter(obj);
//
//      if (pSmg.getHVEdges(filter).iterator().hasNext()) {
////        pLogger.log(Level.SEVERE, "SMG inconsistent: invalid object has a HVEdge");
//        return false;
//      }
//    }
//
//    return true;
//  }
//
//  /**
//   * Verifies that any fields (as designated by Has-Value edges) do not
//   * exceed the boundary of the object.
//   *
//   * @param pLogger A logger to record results
//   * @param pObject An object to verify
//   * @param pSmg A SMG to verify
//   * @return True, if {@link pObject} in {@link pSmg} satisfies all consistency criteria. False
// otherwise.
//   */
//  private static boolean checkSingleFieldConsistency(final SMGObject pObject, final SMG pSmg) {
//
//    // For all fields in the object, verify that sizeof(type)+field_offset < object_size
//    SMGEdgeHasValueFilter filter = SMGEdgeHasValueFilter.objectFilter(pObject);
//
//    for (SMGEdgeHasValue hvEdge : pSmg.getHVEdges(filter)) {
//      if ((hvEdge.getOffset() + hvEdge.getSizeInBytes()) > pObject.getSize()) {
////        pLogger.log(Level.SEVERE, "SMG inconistent: field exceedes boundary of the object");
////        pLogger.log(Level.SEVERE, "Object: ", pObject);
////        pLogger.log(Level.SEVERE, "Field: ", hvEdge);
//        return false;
//      }
//    }
//    return true;
//  }
//
//  /**
//   * Verify all objects satisfy the Field Consistency criteria
//   * @param pLogger A logger to record results
//   * @param pSmg A SMG to verify
//   * @return True, if {@link pSmg} satisfies all consistency criteria. False otherwise.
//   */
//  private static boolean verifyFieldConsistency(final SMG pSmg) {
//    for (SMGObject obj : pSmg.getObjects()) {
//      if (!checkSingleFieldConsistency(obj, pSmg)) {
//        return false;
//      }
//    }
//
//    return true;
//  }
//
//  /**
//   * Verify that the edges are consistent in the SMG
//   *
//   * @param pLogger A logger to record results
//   * @param pSmg A SMG to verify
//   * @param pEdges A set of edges for consistency verification
//   * @return True, if all edges in {@link pEdges} satisfy consistency criteria. False otherwise.
//   */
//  private static boolean verifyEdgeConsistency(final SMG pSmg, final Iterable<? extends SMGEdge>
// pEdges) {
//    ArrayList<SMGEdge> toVerify = new ArrayList<>();
//    Iterables.addAll(toVerify, pEdges);
//
//    while (toVerify.size() > 0) {
//      SMGEdge edge = toVerify.get(0);
//      toVerify.remove(0);
//
//      // Verify that the object assigned to the edge exists in the SMG
//      if (!pSmg.getObjects().contains(edge.getObject())) {
////        pLogger.log(Level.SEVERE, "SMG inconsistent: Edge from a nonexistent object");
////        pLogger.log(Level.SEVERE, "Edge :", edge);
//        return false;
//      }
//
//      // Verify that the value assigned to the edge exists in the SMG
//      if (!pSmg.getValues().contains(edge.getValue())) {
////        pLogger.log(Level.SEVERE, "SMG inconsistent: Edge to a nonexistent value");
////        pLogger.log(Level.SEVERE, "Edge :", edge);
//        return false;
//      }
//
//      // Verify that the edge is consistent to all remaining edges
//      //  - edges of different type are inconsistent
//      //  - two Has-Value edges are inconsistent iff:
//      //    - leading from the same object AND
//      //    - have same type AND
//      //    - have same offset AND
//      //    - leading to DIFFERENT values
//      //  - two Points-To edges are inconsistent iff:
//      //    - different values point to same place (object, offset)
//      //    - same values do not point to the same place
//      for (SMGEdge otherOdge : toVerify) {
//        if (!edge.isConsistentWith(otherOdge)) {
////          pLogger.log(Level.SEVERE, "SMG inconsistent: inconsistent edges");
////          pLogger.log(Level.SEVERE, "First edge:  ", edge);
////          pLogger.log(Level.SEVERE, "Second edge: ", other_edge);
//          return false;
//        }
//      }
//    }
//    return true;
//  }
//
//  private static boolean verifyObjectConsistency(final SMG pSmg) {
//    for (SMGObject obj : pSmg.getObjects()) {
//      try {
//        pSmg.isObjectValid(obj);
//      } catch (IllegalArgumentException e) {
////        pLogger.log(Level.SEVERE, "SMG inconsistent: object does not have validity");
//        return false;
//      }
//
//      if (obj.getSize() < 0) {
////        pLogger.log(Level.SEVERE, "SMG inconsistent: object with size lower than 0");
//        return false;
//      }
//
//      if ((!pSmg.isObjectValid(obj)) && obj.isAbstract()) {
////        pLogger.log(Level.SEVERE, "SMG inconsistent: abstract object is invalid");
////        pLogger.log(Level.SEVERE, obj);
//        return false;
//      }
//    }
//    return true;
//  }
//
//  //TODO: NEQ CONSISTENCY
//}
//

}  // namespace smg
