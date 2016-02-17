#pragma once

#include <string>

namespace smg {

class SMGObjectVisitor;
typedef int ObjectSize;

class SMGObject {
 private:
  static long id_counter_;
  int size_;
  std::string label_;
  long id_;

 public:
  SMGObject(const int size, const std::string label);
  virtual ~SMGObject() {}

  static const SMGObject& GetNullObject();
  std::string GetLabel() const;
  ObjectSize GetSize() const;
  long GetId() const;

  virtual std::string GetClassName() const;
  virtual bool NotNull() const;
  virtual bool IsAbstract() const = 0;
  virtual void Accept(SMGObjectVisitor& visitor) const = 0;
  virtual bool IsMoreGeneral(const SMGObject&) const = 0;
  virtual const SMGObject& Join(const SMGObject& other) const = 0;
};

class SMGNullObject : public SMGObject {
 public:
  SMGNullObject() : SMGObject(0, "NULL") {}

  bool NotNull() const override;
  bool IsAbstract() const override { return false; }
  void Accept(SMGObjectVisitor& visitor) const override { (void)visitor; }
  bool IsMoreGeneral(const SMGObject&) const override { return false; }
  const SMGObject& Join(const SMGObject& other) const override { return other; }
};

}  // namespace smg
