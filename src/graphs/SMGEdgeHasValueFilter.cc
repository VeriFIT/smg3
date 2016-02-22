/*
    Created by Viktor Malik on 19.2.2016.
*/

#include "SMGEdgeHasValueFilter.hh"
#include "graphs/SMG.hh"

namespace smg {

SMGEdgeHasValueFilter::SMGEdgeHasValueFilter()
    : object_(nullptr), value_(SMGValue::GetInvalidValue()), value_complement_(false), offset_(-1),
      type_(SMGCType::GetInvalidType()) {
}

SMGEdgeHasValueFilter SMGEdgeHasValueFilter::ObjectFilter(const SMGObjectPtr& object) {
  SMGEdgeHasValueFilter filter;
  filter.FilterByObject(object);
  return filter;
}
const SMGEdgeHasValueFilter& SMGEdgeHasValueFilter::FilterByObject(const SMGObjectPtr object) {
  object_ = object;
  return *this;
}
const SMGEdgeHasValueFilter& SMGEdgeHasValueFilter::FilterHavingValue(const SMGValue value) {
  value_ = value;
  value_complement_ = false;
  return *this;
}
const SMGEdgeHasValueFilter& SMGEdgeHasValueFilter::FilterNotHavingValue(const SMGValue value) {
  value_ = value;
  value_complement_ = true;
  return *this;
}

const SMGEdgeHasValueFilter& SMGEdgeHasValueFilter::FilterAtOffset(const long offset) {
  offset_ = offset;
  return *this;
}

const SMGEdgeHasValueFilter& SMGEdgeHasValueFilter::FilterByType(const SMGCType& type) {
  type_ = type;
  return *this;
}

bool SMGEdgeHasValueFilter::HoldsFor(const SMGEdgeHasValue& edge) const {
  if (object_ && object_ != edge.GetObject()) {
    return false;
  }

  if (value_ != SMGValue::GetInvalidValue()) {
    if (value_complement_ && value_ == edge.GetValue()) {
      return false;
    } else if (!value_complement_ && value_ != edge.GetValue()) {
      return false;
    }
  }

  if (offset_ != -1 && offset_ != edge.GetOffset()) {
    return false;
  }

  if (type_ != SMGCType::GetInvalidType() && type_ != edge.GetType()) {
    return false;
  }

  return true;
}

const SMGEntitySet<const SMGEdgeHasValue> SMGEdgeHasValueFilter::FilterSet(
    const SMGEntitySet<const SMGEdgeHasValue>& edges) const {
  SMGEntitySet<const SMGEdgeHasValue> return_set;
  for (auto edge : edges) {
    if (HoldsFor(*edge)) {
      return_set.add(edge);
    }
  }
  return return_set;
}

bool SMGEdgeHasValueFilter::operator()(const SMGEdgeHasValuePtr& edge) {
  return HoldsFor(*edge);
}

}  // namespace smg
