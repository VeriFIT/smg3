#include "SMGObject.hh"

SMGObject::SMGObject(const int pSize, const std::string pLabel)
   : size(pSize), label(pLabel) {}

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