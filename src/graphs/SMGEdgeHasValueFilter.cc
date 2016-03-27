/*
    Created by Viktor Malik on 19.2.2016.
*/

#include <algorithm>
#include <iterator>
#include <functional>

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
SMGEdgeHasValueFilter& SMGEdgeHasValueFilter::FilterByObject(const SMGObjectPtr object) {
  object_ = object;
  return *this;
}
SMGEdgeHasValueFilter& SMGEdgeHasValueFilter::FilterHavingValue(const SMGValue value) {
  value_ = value;
  value_complement_ = false;
  return *this;
}
SMGEdgeHasValueFilter& SMGEdgeHasValueFilter::FilterNotHavingValue(const SMGValue value) {
  value_ = value;
  value_complement_ = true;
  return *this;
}

SMGEdgeHasValueFilter& SMGEdgeHasValueFilter::FilterAtOffset(const long offset) {
  offset_ = offset;
  return *this;
}

SMGEdgeHasValueFilter& SMGEdgeHasValueFilter::FilterByType(const SMGCType& type) {
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
    }
    if (!value_complement_ && value_ != edge.GetValue()) {
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
  std::copy_if(
    edges.begin(),
    edges.end(),
    std::inserter(return_set.set(), return_set.begin()),
    *this);
  return return_set;
}

bool SMGEdgeHasValueFilter::operator()(const SMGEdgeHasValuePtr& edge) const {
  return HoldsFor(*edge);
}

}  // namespace smg
