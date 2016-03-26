#pragma once

#include <string>
#include "objects/SMGObject.hh"

namespace smg {

class SMGObjectVisitor;

class SMGRegion : public SMGObject {
 public:
  SMGRegion(const int size, const std::string& label);

  virtual std::string ToString() const;
  bool PropertiesEqual(const SMGRegion& other) const;

  std::string GetClassName() const override;
  bool IsAbstract() const override;
  void Accept(SMGObjectVisitor& visitor) const override;
  bool IsMoreGeneral(const SMGObject& other) const override;
  SMGObjectPtr Join(const SMGObject& other) const override;
};

typedef std::shared_ptr<const SMGRegion> SMGRegionPtr;

}  // namespace smg
