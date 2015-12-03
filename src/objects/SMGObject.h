#include <string>

class SMGObject
{
private:
   int size;
   std::string label;

public:
   SMGObject(const int, const std::string);
   virtual ~SMGObject() {};
   static const SMGObject& getNullObject();
   const std::string getLabel();
   const int getSize();
   virtual bool notNull() const = 0;

   virtual bool isAbstract() const = 0;
   virtual bool isMoreGeneral(const SMGObject &) const = 0;
   virtual const SMGObject &join(const SMGObject &) const = 0;
};

class SMGNullObject : public SMGObject
{
public:
   SMGNullObject() : SMGObject(0, "NULL") {}

   virtual bool isAbstract() const override {return false;}
   virtual bool isMoreGeneral(const SMGObject &) const override {return false;}
   virtual const SMGObject &join(const SMGObject &pOther) const override {return pOther;}
   virtual bool notNull() const override;
};
