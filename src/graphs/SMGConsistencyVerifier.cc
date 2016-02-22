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


/**
  * Verifies that invalid regions do not have any Has-Value edges, as this
  * is forbidden in consistent SMGs
  */
bool SMGConsistencyVerifier::VerifyInvalidRegionsHaveNoHVEdges(const SMG& smg) {
  for (auto obj : smg.GetObjects()) {
    if (smg.IsObjectValid(obj)) {
      continue;
    }

    SMGEdgeHasValueFilter filter = SMGEdgeHasValueFilter::ObjectFilter(obj);
    if (smg.GetHVEdges(filter).size() > 0) {
      return false;
    }
  }

  return true;
}

/**
  * Verifies that any fields (as designated by Has-Value edges) do not
  * exceed the boundary of the object.
  */
bool SMGConsistencyVerifier::CheckSingleFieldConsistency(const SMGObjectPtr& object,
                                                         const SMG& smg) {
  SMGEdgeHasValueFilter filter = SMGEdgeHasValueFilter::ObjectFilter(object);

  for (auto hv_edge : smg.GetHVEdges(filter)) {
    if ((hv_edge->GetOffset() + hv_edge->GetSizeInBytes()) > object->GetSize()) {
      return false;
    }
  }

  return true;
}

/**
  * Verify all objects satisfy the Field Consistency criteria
  */
bool SMGConsistencyVerifier::VerifyFieldConsistency(const SMG& smg) {
  for (auto obj : smg.GetObjects()) {
    if (!CheckSingleFieldConsistency(obj, smg)) {
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

bool SMGConsistencyVerifier::Verify(const SMG& smg) {
  bool to_return = true;
  to_return = to_return && VerifyNullObject(smg);
  to_return = to_return && VerifyInvalidRegionsHaveNoHVEdges(smg);
  to_return = to_return && VerifyFieldConsistency(smg);
  to_return = to_return && VerifyEdgeConsistency(smg);
  to_return = to_return && VerifyObjectConsistency(smg);
  return to_return;
}

SMGConsistencyVerifier::SMGConsistencyVerifier() { }

SMGConsistencyVerifier::~SMGConsistencyVerifier() { }

}  // namespace smg
