#pragma once

#include <memory>
#include <string>

class SMGObjectVisitor;
typedef int TObjectSize;

class SMGObject;
typedef std::shared_ptr<const SMGObject> SMGObjectPtr;

class SMGObject {
private:
    static long id_counter;
    int size;
    const std::string label;
    long id;

public:
    SMGObject(const int pSize, const std::string &pLabel);
    virtual ~SMGObject();

    virtual std::string getClassName() const;

    const std::string& getLabel() const;
    TObjectSize getSize() const;
    long getId() const;

    virtual bool isAbstract() const = 0;
    virtual void accept(SMGObjectVisitor & visitor) const = 0;
    virtual bool isMoreGeneral(const SMGObject& pOther) const = 0;
    virtual bool notNull() const;
    virtual SMGObjectPtr join(const SMGObject &pOther) const = 0;
    bool operator==(const SMGObject &pOther) const;
    bool operator!=(const SMGObject &pOther) const;
};

class SMGNullObject: public SMGObject {
private:
    const static SMGObjectPtr NULL_OBJECT;

    SMGNullObject();

public:
    static const SMGObjectPtr getNullObject();
    bool isAbstract() const override;
    void accept(SMGObjectVisitor &pVisitor) const override;
    bool isMoreGeneral(const SMGObject &pOther) const override;
    bool notNull() const override;
    virtual SMGObjectPtr join(const SMGObject &pOther) const override;
};
