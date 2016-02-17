#include "SMGRegion.hh"
#include <string>
#include "exceptions/UnsupportedOperationException.hh"
#include "objects/SMGObjectVisitor.hh"

namespace smg {

SMGRegion::SMGRegion(const int size, const std::string& label) : SMGObject(size, label) {}

std::string SMGRegion::ToString() const {
  return "REGION( " + GetLabel() + ", " + std::to_string(GetSize()) + "b)";
}

std::string SMGRegion::GetClassName() const { return std::string("SMGRegion"); }

bool SMGRegion::PropertiesEqual(const SMGRegion& other) const {
  if (&other == this) {
    return true;
  }
  if (&other == nullptr) {
    return false;
  }
  if (GetLabel() != other.GetLabel()) {
    return false;
  }
  if (GetSize() != other.GetSize()) {
    return false;
  }
  return true;
}

bool SMGRegion::IsAbstract() const { return false; }

void SMGRegion::Accept(SMGObjectVisitor& visitor) const { visitor.Visit(*this); }

bool SMGRegion::IsMoreGeneral(const SMGObject& other __attribute__((unused))) const {
  return false;
}

SMGObjectPtr SMGRegion::Join(const SMGObject& other) const {
  if (other.IsAbstract()) {
    return other.Join(*this);
  } else if (GetSize() == other.GetSize() || !other.NotNull()) {
    return std::make_shared<const SMGRegion>(*this);
  }
  throw UnsupportedOperationException("join() called on incompatible SMGRegions");
}

}  // namespace smg
