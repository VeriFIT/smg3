#pragma once

#include <memory>
#include <string>

namespace smg {

class SMGObjectId {
private:
  long id;
public:
  SMGObjectId() : id{0} { }
  SMGObjectId(long id) : id{id} { }
  SMGObjectId operator++(int) { auto copy = *this; id++; return copy; }
  SMGObjectId operator++() { id++; return *this; }
  bool operator==(const SMGObjectId& other) const { return this->id == other.id; }
  bool operator<(const SMGObjectId& other) const { return this->id < other.id; }
  bool operator>(const SMGObjectId& other) const { return this->id > other.id; }
  bool operator<=(const SMGObjectId& other) const { return this->id <= other.id; }
  bool operator>=(const SMGObjectId& other) const { return this->id >= other.id; }
  operator long() const { return id; }
};

class SMGObjectVisitor;
typedef int ObjectSize;

class SMGObject;
typedef std::shared_ptr<const SMGObject> SMGObjectPtr;

class SMGObject {
 private:
  static SMGObjectId id_counter_;
  ObjectSize size_;
  const std::string label_;
  SMGObjectId id_;

 public:
  SMGObject(const ObjectSize size, const std::string& label);
  virtual ~SMGObject();

  const std::string& GetLabel() const;
  ObjectSize GetSize() const;
  SMGObjectId GetId() const;

  virtual std::string ToString() const;
  virtual std::string GetClassName() const;
  virtual bool NotNull() const;
  virtual bool IsAbstract() const = 0;
  virtual void Accept(SMGObjectVisitor& visitor) const = 0;
  virtual bool IsMoreGeneral(const SMGObject& other) const = 0;
  virtual SMGObjectPtr Join(const SMGObject& other) const = 0;
  bool operator==(const SMGObject& other) const;
  bool operator!=(const SMGObject& other) const;
};



class SMGNullObject : public SMGObject {
 private:
  static const SMGObjectPtr NULL_OBJECT;

  SMGNullObject();

 public:
  static const SMGObjectPtr GetNullObject();
  bool IsAbstract() const override;
  void Accept(SMGObjectVisitor& visitor) const override;
  bool IsMoreGeneral(const SMGObject& other) const override;
  bool NotNull() const override;
  SMGObjectPtr Join(const SMGObject& other) const override;
};

}  // namespace smg
