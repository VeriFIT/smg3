#include "SMGObject.hh"

namespace smg {

long SMGObject::id_counter_ = 0;

SMGObject::SMGObject(const int size, const std::string label) : size_(size), label_(label) {
  id_ = SMGObject::id_counter_++;
}

const SMGObject& SMGObject::GetNullObject() {
  static SMGNullObject null_object;
  return null_object;
}

std::string SMGObject::GetLabel() const { return label_; }

int SMGObject::GetSize() const { return size_; }

bool SMGObject::NotNull() const { return true; }

bool SMGNullObject::NotNull() const { return false; }

// ReSharper disable once CppMemberFunctionMayBeStatic
std::string SMGObject::GetClassName() const { return std::string("SMGObject"); }

long SMGObject::GetId() const { return id_; }

}  // namespace smg
