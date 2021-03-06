#pragma once

#include "graphs/SMGValue.hh"
#include "objects/SMGObject.hh"

namespace smg {

class SMGEdge {
  const SMGValue value_;
  const SMGObjectPtr object_;

 protected:
  SMGEdge(const SMGValue& value, const SMGObjectPtr& object);
  bool IsConsistentWith(const SMGEdge& other_edge) const;

 public:
  const SMGValue& GetValue() const;
  SMGObjectPtr GetObject() const;
};

}  // namespace smg
