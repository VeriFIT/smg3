/*
 * SMGConsistencyVerifier.cc
 *
 *  Created on: Jan 28, 2016
 *      Author: pmuller
 */

#include "SMGConsistencyVerifier.hh"
#include <list>
#include "exceptions/IllegalArgumentException.hh"

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

  if (smg.IsObjectValid(smg.GetNullObject())) {
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

  if (smg.GetNullObject()->GetSize() != 0) {
    return false;
  }

  return true;
}

bool SMGConsistencyVerifier::VerifyObjectConsistency(const SMG& smg) {
  auto objects = smg.GetObjects();
  for (SMGObjectPtr obj : objects) {
    try {
      smg.IsObjectValid(obj);
    } catch (IllegalArgumentException e) {
      return false;
    }

    if (obj->GetSize() < 0) {
      return false;
    }

    if (!smg.IsObjectValid(obj) && obj->IsAbstract()) {
      return false;
    }
  }

  return true;
}

bool SMGConsistencyVerifier::VerifyGeneralEdgeConsistency(const SMG& smg, const SMGEdge& edge) {
  // Verify that the object assigned to the edge exists in the SMG
  if (!smg.GetObjects().contains(edge.GetObject())) {
    return false;
  }

  // Verify that the value assigned to the edge exists in the SMG
  if (smg.GetValues().find(edge.GetValue()) == smg.GetValues().end()) {
    return false;
  }

  return true;
}

bool SMGConsistencyVerifier::VerifyEdgeConsistency(const SMG& smg) {
  auto hv_edges = smg.GetHVEdges();
  std::list<SMGEdgeHasValuePtr> to_verify_hv(hv_edges.begin(), hv_edges.end());

  while (to_verify_hv.size() > 0) {
    auto edge = to_verify_hv.front();
    to_verify_hv.pop_front();
    if (!VerifyGeneralEdgeConsistency(smg, *edge)) {
      return false;
    }

    // Verify that the edge is consistent to all remaining edges
    //  - edges of different type are inconsistent
    //  - two Has-Value edges are inconsistent iff:
    //    - leading from the same object AND
    //    - have same type AND
    //    - have same offset AND
    //    - leading to DIFFERENT values
    for (auto other_edge : to_verify_hv) {
      if (!edge->IsConsistentWith(*other_edge)) {
        return false;
      }
    }
  }

  auto pt_edges = smg.GetPTEdges();
  std::list<SMGEdgePointsToPtr> to_verify_pt;
  for (auto pt : pt_edges) {
    to_verify_pt.push_back(pt.second);
  }

  while (to_verify_pt.size() > 0) {
    auto edge = to_verify_pt.front();
    to_verify_pt.pop_front();
    if (!VerifyGeneralEdgeConsistency(smg, *edge)) {
      return false;
    }

    // Verify that the edge is consistent to all remaining edges
    //  - edges of different type are inconsistent
    //  - two Points-To edges are inconsistent iff:
    //    - different values point to same place (object, offset)
    //    - same values do not point to the same place
    for (auto other_edge : to_verify_pt) {
      if (!edge->IsConsistentWith(*other_edge)) {
        return false;
      }
    }
  }

  return true;
}

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
//  //TODO: NEQ CONSISTENCY
//}

bool SMGConsistencyVerifier::Verify(const SMG& smg) {
  bool to_return = true;
  to_return = to_return && VerifyNullObject(smg);
  to_return = to_return && VerifyObjectConsistency(smg);
  to_return = to_return && VerifyEdgeConsistency(smg);
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
  //
  //  //    pLogger.log(Level.FINEST, "Ending consistency check of a SMG");
  //
  //      return toReturn;
  //    }
}

SMGConsistencyVerifier::SMGConsistencyVerifier() { }
SMGConsistencyVerifier::~SMGConsistencyVerifier() { }

}  // namespace smg
