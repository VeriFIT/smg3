#include "SMGObject.hh"

SMGObject::SMGObject(const int pSize, const std::string pLabel)
   : size(pSize), label(pLabel) {}

// ReSharper disable once CppMemberFunctionMayBeStatic
std::string SMGObject::getClassName() const
{
  return std::string("SMGObject");
}

const SMGObject & SMGObject::getNullObject()
{
   static SMGNullObject nullObject;
   return nullObject;
}

std::string SMGObject::getLabel() const
{
   return label;
}

int SMGObject::getSize() const
{
   return size;
}

bool SMGObject::notNull() const{
  return true;
}

bool SMGNullObject::notNull() const{
  return false;
}
