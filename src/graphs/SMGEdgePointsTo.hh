#pragma once

#include <string>
#include "graphs/SMGEdge.hh"
#include "graphs/SMGValue.hh"
#include "objects/SMGObject.hh"

namespace smg {

class SMGEdgePointsTo : public SMGEdge {
 private:
  long offset_;

 public:
  SMGEdgePointsTo(const SMGValue& value, const SMGObject& object, int offset);
  long GetOffset() const;
  bool IsConsistentWith(const SMGEdgePointsTo& other_edge) const;
};

}  // namespace smg
