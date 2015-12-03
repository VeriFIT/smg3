#include <string>

class SMGObject
{
private:
   int size;
   std::string label;

public:
   SMGObject(const int, const std::string);

   static const SMGObject &getNullObject();
   const std::string getLabel();
   const int getSize();

   virtual bool isAbstract() = 0;
   virtual bool isMoreGeneral(SMGObject &) = 0;
   virtual SMGObject &join(SMGObject &) = 0;
};

class SMGNullObject : public SMGObject
{
public:
   SMGNullObject() : SMGObject(0, "NULL") {}

   bool isAbstract() {return false;}
   bool isMoreGeneral(SMGObject &) {return false;}
   SMGObject &join(SMGObject &pOther) {return pOther;}
};