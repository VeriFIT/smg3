#pragma once

#include <string>

class SMGObjectVisitor;
typedef int TObjectSize;

class SMGObject
{
private:
  static long id_counter;
  int size;
  std::string label;
  long id;

public:
  SMGObject(const int, const std::string);
  virtual ~SMGObject() {}

  virtual std::string getClassName() const;

  static const SMGObject & getNullObject();
  std::string getLabel() const;
  TObjectSize getSize() const;
  long getId() const;

  virtual bool isAbstract() const = 0;
  virtual void accept(SMGObjectVisitor & visitor) const = 0;
  virtual bool isMoreGeneral(const SMGObject &) const = 0;
  virtual const SMGObject & join(const SMGObject & pOther) const = 0;
  virtual bool notNull() const;
};

class SMGNullObject : public SMGObject
{
public:
  SMGNullObject() : SMGObject(0, "NULL") {}

  bool isAbstract() const override { return false; }
  void accept(SMGObjectVisitor & visitor) const override { (void)visitor; }
  bool isMoreGeneral(const SMGObject &) const override { return false; }
  const SMGObject & join(const SMGObject & pOther) const override { return pOther; }
  bool notNull() const override;
};
