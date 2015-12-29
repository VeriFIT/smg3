#include "SMGRegion.hh"
#include "SMGObjectVisitor.hh"
#include <UnsupportedOperationException.hh>
#include <string>

SMGRegion::SMGRegion(const int pSize, std::string pLabel)
  :SMGObject(pSize, pLabel)
{}

std::string SMGRegion::toString() const
{
  return "REGION( " + getLabel() + ", " + std::to_string(getSize()) + "b)";
}

std::string SMGRegion::getClassName() const
{
  return std::string("SMGRegion");
}

bool SMGRegion::propertiesEqual(const SMGRegion & pOther) const
{
  if (&pOther == this) {
    return true;
  }
  if (&pOther == nullptr) {
    return false;
  }

  if (getLabel() != pOther.getLabel()) {
    return false;
  }

  if (getSize() != pOther.getSize()) {
    return false;
  }
  return true;
}

bool SMGRegion::isAbstract() const
{
  return false;
}

void SMGRegion::accept(SMGObjectVisitor& visitor) const
{
  visitor.visit(*this);
}

const SMGObject& SMGRegion::join(const SMGObject& pOther) const
{
  if (pOther.isAbstract()) {
    // I am concrete, and the other is abstract: the abstraction should
    // know how to join with me
    return pOther.join(*this);
  }
  else if (getSize() == pOther.getSize()) {
    return *this; //odstranen copy-like-konstruktor
  }
  throw UnsupportedOperationException("join() called on incompatible SMGObjects");
}

bool SMGRegion::isMoreGeneral(const SMGObject&) const
{
  return false;
}
