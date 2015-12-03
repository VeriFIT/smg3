#include <string>

typedef int TObjectSize;

class SMGObject
{
private:
   int size;
   std::string label;

public:
   SMGObject(const int, const std::string);

   static const SMGObject & getNullObject();
	 std::string getLabel() const;
	 TObjectSize getSize() const;

   virtual bool isAbstract() const = 0;
   virtual bool isMoreGeneral(SMGObject &) const = 0;
   virtual SMGObject &join(SMGObject &) = 0;
};

class SMGNullObject : public SMGObject
{
public:
   SMGNullObject() : SMGObject(0, "NULL") {}

   bool isAbstract() const {return false;}
   bool isMoreGeneral(SMGObject &) const {return false;}
   SMGObject &join(SMGObject & pOther) {return pOther;}
};