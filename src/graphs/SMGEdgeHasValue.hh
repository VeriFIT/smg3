#pragma once

#include <string>
#include "graphs/SMGEdge.hh"
#include "graphs/SMGValue.hh"
#include "objects/SMGObject.hh"
#include "types/SMGCType.hh"

namespace smg {

class SMGEdgeHasValue : public SMGEdge {
 private:
  const long offset_;
  const SMGCType& type_;

 public:
  SMGEdgeHasValue(const SMGCType& type,
                  const int offset,
                  const SMGObject& object,
                  const SMGValue& value);
  long GetOffset() const;
  const SMGCType& GetType() const;
  int GetSizeInBytes() const;
  bool IsConsistentWith(const SMGEdgeHasValue& other) const;
  bool OverlapsWith(const SMGEdgeHasValue& other) const;
  bool OverlapsWith(const int other_start, const int other_end) const;
  bool IsCompatibleField(const SMGEdgeHasValue& other) const;
  bool IsCompatibleFieldOnSameObject(const SMGEdgeHasValue& other) const;
};

}  // namespace smg
