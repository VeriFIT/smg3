#pragma once

#include <string>

namespace smg {
  
class SMGValueId {
private:
  long id;
public:
  SMGValueId() : id{0} { }
  SMGValueId(long id) : id{id} { }
  SMGValueId operator++(int) { auto copy = *this; id++; return copy; }
  SMGValueId operator++() { id++; return *this; }
  bool operator==(const SMGValueId& other) const { return this->id == other.id; }
  bool operator<(const SMGValueId& other) const { return this->id < other.id; }
  bool operator>(const SMGValueId& other) const { return this->id > other.id; }
  bool operator<=(const SMGValueId& other) const { return this->id <= other.id; }
  bool operator>=(const SMGValueId& other) const { return this->id >= other.id; }
  operator long() const { return id; }
};

class SMGValue {
 private:
  static SMGValueId id_counter_;
  static const SMGValue NULL_VALUE;
  static const SMGValue INVALID_VALUE;
  static const SMGValue UNKNOWN_VALUE;
  SMGValueId id_;

  explicit SMGValue(const SMGValueId id);

 public:
  static const SMGValue GetNewValue();

  static const SMGValue& GetNullValue();
  static const SMGValue& GetInvalidValue();
  static const SMGValue& GetUnknownValue();

  SMGValueId GetId() const;

  bool operator<(const SMGValue& other) const;
  bool operator==(const SMGValue& other) const;
  bool operator!=(const SMGValue& other) const;
};

}  // namespace smg
