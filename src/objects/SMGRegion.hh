#include <string>

#include "SMGObject.hh"

class SMGRegion : public SMGObject
{
public:
  SMGRegion(const int pSize, const std::string pLabel);
  std::string toString() const;

  bool propertiesEqual(const SMGRegion & pOther) const;

  bool isAbstract() const override;

  //void accept(const SMGObjectVisitor visitor);

  const SMGObject& join(const SMGObject & pOther) const override;

  bool isMoreGeneral(const SMGObject&) const override;

};

class SMGObjectVisitor {};