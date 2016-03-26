#include "SMG.hh"
#include <algorithm>
#include <iterator>
#include <vector>
#include "exceptions/IllegalArgumentException.hh"

namespace smg {

SMG::SMG() {
  SMGEdgePointsToPtr null_pointer = std::make_shared<SMGEdgePointsTo>(null_value_, null_object_, 0);
  AddObject(null_object_);
  object_validity_[null_object_->GetId()] = false;
  AddValue(null_value_);
  AddPointsToEdge(null_pointer);
}

SMG::~SMG() { }

void SMG::AddObject(const SMGObjectPtr& object) { AddObject(object, true); }

void SMG::AddObject(const SMGObjectPtr& object, const bool validity) {
  objects_.add(object);
  object_validity_[object->GetId()] = validity;
}

void SMG::AddValue(const SMGValue& value) { values_.insert(value); }

void SMG::AddPointsToEdge(const SMGEdgePointsToPtr& edge) { pt_edges_[edge->GetValue()] = edge; }

void SMG::AddHasValueEdge(const SMGEdgeHasValuePtr& edge) { hv_edges_.add(edge); }

SMGObjectPtr SMG::GetNullObject() const { return null_object_; }

const SMGValue SMG::GetNullValue() const { return null_value_; }

const SMGEntitySet<const SMGObject>& SMG::GetObjects() const { return objects_; }

const std::set<SMGValue>& SMG::GetValues() const { return values_; }

const std::map<SMGValue, SMGEdgePointsToPtr>& SMG::GetPTEdges() const { return pt_edges_; }

const SMGEntitySet<const SMGEdgeHasValue>& SMG::GetHVEdges() const { return hv_edges_; }

const SMGEntitySet<const SMGEdgeHasValue> SMG::GetHVEdges(
  const SMGEdgeHasValueFilter& filter) const {
  SMGEntitySet<const SMGEdgeHasValue> to_return;
  std::copy_if(
    hv_edges_.begin(),
    hv_edges_.end(),
    std::inserter(to_return.set(), to_return.begin()),
    filter);
  return to_return;
}

const SMGEdgeHasValuePtr SMG::GetUniqueHV(const SMGEdgeHasValueFilter& filter, const bool check) {
  auto hv_edges = GetHVEdges(filter);
  if (check && hv_edges.size() > 1) {
    throw IllegalArgumentException("Applying filter does not result in unique HV edge");
  }
  return *(hv_edges.begin());
}

const SMGObjectPtr SMG::GetObjectPointedBy(const SMGValue& value) const {
  SMGEdgePointsToPtr edge = pt_edges_.find(value)->second;
  return edge->GetObject();
}

void SMG::RemoveObject(const SMGObjectPtr& object) {
  objects_.remove(object);
  object_validity_.erase(object->GetId());
}

void SMG::RemoveValue(const SMGValue& value) { values_.erase(value); }

void SMG::RemovePointsToEdge(const SMGEdgePointsToPtr& edge) { pt_edges_.erase(edge->GetValue()); }

void SMG::RemoveHasValueEdge(const SMGEdgeHasValuePtr& edge) { hv_edges_.remove(edge); }

void SMG::RemoveObjectAndEdges(const SMGObjectPtr& object) {
  RemoveObject(object);
  for (auto hv_iter = hv_edges_.begin(); hv_iter != hv_edges_.end();) {
    if ((*hv_iter)->GetObject() == object) {
      hv_iter = hv_edges_.remove(hv_iter);
    } else {
      ++hv_iter;
    }
  }
  for (auto pt_iter = pt_edges_.begin(); pt_iter != pt_edges_.end();) {
    if (pt_iter->second->GetObject() == object) {
      pt_iter = pt_edges_.erase(pt_iter);
    } else {
      ++pt_iter;
    }
  }
}

void SMG::SetValidity(const SMGObjectPtr& object, const bool validity) {
  if (!objects_.contains(object)) {
    std::string msg = "Object [" + object->GetLabel() + "] is not in SMG";
    throw IllegalArgumentException(msg.c_str());
  }

  object_validity_[object->GetId()] = validity;
}

bool SMG::IsObjectValid(const SMGObjectPtr& object) const {
  if (!objects_.contains(object)) {
    std::string msg = "Object [" + object->GetLabel() + "] is not in SMG";
    throw IllegalArgumentException(msg.c_str());
  }

  return object_validity_.at(object->GetId());
}

bool SMG::IsObjectValid(const SMGObject& object) const {
  bool contains = false;
  auto selector = [](const SMGObjectPtr& oPtr)->const SMGObject&{return *oPtr; };
  for (const auto& optr : objects_) {
    contains |= selector(optr) == object;
  }
  if (contains) {
    std::string msg = "Object [" + object.GetLabel() + "] is not in SMG";
    throw IllegalArgumentException(msg.c_str());
  }

  return object_validity_.at(object.GetId());
}

std::vector<bool> SMG::GetNullBytesForObject(const SMGObjectPtr& obj) const {
  //TODO(anyone) assert on obj->GetSize() >= 0 ?
  std::vector<bool> bs = std::vector<bool>(static_cast<size_t>(obj->GetSize()), false);

  auto filt = SMGEdgeHasValueFilter::ObjectFilter(obj).FilterHavingValue(SMGValue::GetNullValue());

  for (auto edge : GetHVEdges(filt)) {
    //TODO(anyone) is it possible to optimize? maybe boost::dynamic_bitset
    for (
      auto b = bs.begin() + edge->GetOffset();
      b < bs.begin() + edge->GetOffset() + edge->GetSizeInBytes();
      b++) {
      *b = true;
    }
  }

  return bs;
}

bool SMG::IsCoveredByNullifiedBlocks(
  const SMGEdgeHasValuePtr& edge) const {
  return IsCoveredByNullifiedBlocks(edge->GetObject(), edge->GetOffset(), edge->GetSizeInBytes());
}
bool SMG::IsCoveredByNullifiedBlocks(
  const SMGObjectPtr& obj,
  long offset,
  const SMGCType& type) const {
  return IsCoveredByNullifiedBlocks(obj, offset, type.GetSize());
}
//TODO(anyone) what kind of size?
bool SMG::IsCoveredByNullifiedBlocks(
  const SMGObjectPtr& obj,
  long offset,
  int size) const {
  auto bs = GetNullBytesForObject(obj);
  //TODO(anyone): what type?
  //min index of last NULL byte
  long expectedMinNull = (offset + size);
  //index of first not-guaranted-to-be-NULL in bs
  auto firstNotNull = std::find(bs.cbegin(), bs.cend(), false) - bs.cbegin();

  return (firstNotNull >= expectedMinNull);
}

}  // namespace smg
