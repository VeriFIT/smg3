#include "SMGObject.h"

SMGObject::SMGObject(const int pSize, const std::string pLabel)
   : size(pSize), label(pLabel) {}

const SMGObject &SMGObject::getNullObject()
{
   static SMGNullObject nullObject;
   return nullObject;
}

const std::string SMGObject::getLabel()
{
   return label;
}

const int SMGObject::getSize()
{
   return size;
}

bool SMGObject::notNull() const{
	return true;
}

bool SMGNullObject::notNull() const{
	return false;
}
