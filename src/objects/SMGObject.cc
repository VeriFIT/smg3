#include "SMGObject.hh"

namespace smg {

long SMGObject::id_counter_ = 1;
const SMGObjectPtr SMGNullObject::NULL_OBJECT = SMGObjectPtr(new SMGNullObject());

const SMGObjectPtr SMGNullObject::GetNullObject() { return SMGNullObject::NULL_OBJECT; }

SMGObject::SMGObject(const ObjectSize size, const std::string& label) : size_(size), label_(label) {
  id_ = SMGObject::id_counter_++;
}

SMGObject::~SMGObject() {}

const std::string& SMGObject::GetLabel() const { return label_; }

ObjectSize SMGObject::GetSize() const { return size_; }

long SMGObject::GetId() const { return id_; }

// ReSharper disable once CppMemberFunctionMayBeStatic
std::string SMGObject::GetClassName() const { return std::string("SMGObject"); }

bool SMGObject::operator==(const SMGObject& other) const { return id_ == other.id_; }

bool SMGObject::operator!=(const SMGObject& other) const { return !(*this == other); }

bool SMGObject::NotNull() const { return true; }

std::string SMGNullObject::ToString() const {
  return "OBJECT( " + GetLabel() + ", " + std::to_string(GetSize()) + "b)";
}

bool SMGNullObject::NotNull() const { return false; }

SMGNullObject::SMGNullObject() : SMGObject(0, "NULL") {}

bool SMGNullObject::IsAbstract() const { return false; }

void SMGNullObject::Accept(SMGObjectVisitor&) const {}

bool SMGNullObject::IsMoreGeneral(const SMGObject&) const {
  return false;
}

SMGObjectPtr SMGNullObject::Join(const SMGObject& other) const {
  if (other.NotNull()) {
    return other.Join(*this);
  } else {
    return SMGNullObject::GetNullObject();
  }
}

}  // namespace smg
