#include "SMGRegion.hh"
#include <string>
#include "exceptions/UnsupportedOperationException.hh"
#include "objects/SMGObjectVisitor.hh"

namespace smg {

SMGRegion::SMGRegion(const int size, std::string label) : SMGObject(size, label) {}

std::string SMGRegion::ToString() const {
  return "REGION( " + GetLabel() + ", " + std::to_string(GetSize()) + "b)";
}

std::string SMGRegion::GetClassName() const { return std::string("SMGRegion"); }

bool SMGRegion::PropertiesEqual(const SMGRegion& other) const {
  if (&other == this) {
    return true;
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

const SMGObject& SMGRegion::Join(const SMGObject& other) const {
  if (other.IsAbstract()) {
    // I am concrete, and the other is abstract: the abstraction should
    // know how to join with me
    return other.Join(*this);
  } else if (GetSize() == other.GetSize()) {
    return *this;  // odstranen copy-like-konstruktor
  }
  throw UnsupportedOperationException("join() called on incompatible SMGObjects");
}

bool SMGRegion::IsMoreGeneral(const SMGObject&) const { return false; }

}  // namespace smg
