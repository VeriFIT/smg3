/*
    Created by Viktor Malik on 19.2.2016.
*/

#pragma once

#include "graphs/SMGEdgeHasValue.hh"
#include "graphs/SMGValue.hh"
#include "objects/SMGObject.hh"
#include "types/SMGCType.hh"

namespace smg {

template <class T> class SMGEntitySet;

class SMGEdgeHasValueFilter {
  SMGObjectPtr object_;
  SMGValue value_;
  bool value_complement_;
  long offset_;
  SMGCType type_;

 public:
  static SMGEdgeHasValueFilter ObjectFilter(const SMGObjectPtr& object);

  SMGEdgeHasValueFilter();

  // FluentApi / FluentConfig / Method chaining
  SMGEdgeHasValueFilter& FilterByObject(const SMGObjectPtr object);
  SMGEdgeHasValueFilter& FilterHavingValue(const SMGValue value);
  SMGEdgeHasValueFilter& FilterNotHavingValue(const SMGValue value);
  SMGEdgeHasValueFilter& FilterAtOffset(const long offset);
  SMGEdgeHasValueFilter& FilterByType(const SMGCType& type);

  bool HoldsFor(const SMGEdgeHasValue& edge) const;
  const SMGEntitySet<const SMGEdgeHasValue> FilterSet(
      const SMGEntitySet<const SMGEdgeHasValue>& edges) const;
  bool operator()(const SMGEdgeHasValuePtr& edge) const;
};

}  // namespace smg
