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
  SMGEdgePointsTo(const SMGValue& value, const SMGObjectPtr& object, const int offset);
  long GetOffset() const;
  bool IsConsistentWith(const SMGEdgePointsTo& other) const;

  //TODO(anyone) toString NOT PORTED
  //TODO(anyone) hashCode and equals NOT PORTED - useful for hashset?
};

typedef std::shared_ptr<const SMGEdgePointsTo> SMGEdgePointsToPtr;

}  // namespace smg
