#pragma once

#include <string>
#include "SMGObject.hh"

class SMGObjectVisitor;

class SMGRegion: public SMGObject {
public:
    SMGRegion(const int pSize, const std::string &pLabel);

    std::string toString() const;

    std::string getClassName() const override;

    bool propertiesEqual(const SMGRegion &pOther) const;

    bool isAbstract() const override;
    void accept(SMGObjectVisitor &pVisitor) const override;
    bool isMoreGeneral(const SMGObject &pOther) const override;
};
