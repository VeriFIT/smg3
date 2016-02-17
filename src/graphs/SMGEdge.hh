#pragma once

#include "graphs/SMGValue.hh"
#include "objects/SMGObject.hh"

namespace smg {

class SMGEdge {
  const SMGValue& value_;
  const SMGObject& object_;

 protected:
  SMGEdge(const SMGValue& value, const SMGObject& object);
  bool IsConsistentWith(const SMGEdge& other_edge) const;

 public:
  const SMGValue& GetValue() const;
  const SMGObject& GetObject() const;
};

}  // namespace smg
