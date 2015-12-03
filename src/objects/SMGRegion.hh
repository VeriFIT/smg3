#include <string>

#include "SMGObject.hh"

class SMGRegion : SMGObject
{
public:
	SMGRegion(const int pSize, const std::string pLabel);

	std::string toString();

	bool propertiesEqual(const SMGRegion & pOther) const;

	bool isAbstract() const override;

	//void accept(const SMGObjectVisitor visitor);

};

class SMGObjectVisitor {};