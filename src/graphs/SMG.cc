#include "SMG.hh"

namespace smg {

SMG::SMG() {
  SMGEdgePointsToPtr null_pointer = std::make_shared<SMGEdgePointsTo>(null_value_, null_object_, 0);
  AddObject(null_object_);
  AddValue(null_value_);
  AddPointsToEdge(null_pointer);
}

SMG::~SMG() { }

void SMG::AddObject(const SMGObjectPtr& object) { objects_.add(object); }

void SMG::AddValue(const SMGValue& value) { values_.insert(value); }

void SMG::AddPointsToEdge(const SMGEdgePointsToPtr& edge) { pt_edges_[edge->GetValue()] = edge; }

void SMG::AddHasValueEdge(const SMGEdgeHasValuePtr& edge) { hv_edges_.add(edge); }

SMGObjectPtr SMG::GetNullObject() const { return null_object_; }

const SMGValue SMG::GetNullValue() const { return null_value_; }

const SMGEntitySet<const SMGObject>& SMG::GetObjects() const { return objects_; }

const std::set<SMGValue>& SMG::GetValues() const { return values_; }

const std::map<SMGValue, SMGEdgePointsToPtr>& SMG::GetPTEdges() const { return pt_edges_; }

const SMGEntitySet<const SMGEdgeHasValue>& SMG::GetHVEdges() const { return hv_edges_; }

const SMGObjectPtr SMG::GetObjectPointedBy(const SMGValue& value) const {
  SMGEdgePointsToPtr edge = pt_edges_.find(value)->second;
  return edge->GetObject();
}

void SMG::RemoveObject(const SMGObjectPtr& object) { objects_.remove(object); }

void SMG::RemoveValue(const SMGValue& value) { values_.erase(value); }

void SMG::RemovePointsToEdge(const SMGEdgePointsToPtr& edge) { pt_edges_.erase(edge->GetValue()); }

void SMG::RemoveHasValueEdge(const SMGEdgeHasValuePtr& edge) { hv_edges_.remove(edge); }

}  // namespace smg
