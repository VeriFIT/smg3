#pragma once

namespace smg {

class SMGCType {
 private:
  static const int SIZE_INT = 4;
  static const int SIZE_POINTER = 8;
  static const int SIZE_INVALID = -1;
  int size_;

 public:
  static const SMGCType CreateTypeWithSize(const int size);
  static const SMGCType GetIntType();
  static const SMGCType GetPointerType();
  static const SMGCType GetInvalidType();

  explicit SMGCType(const int size);
  int GetSize() const;

  bool operator==(const SMGCType& other) const;
  bool operator!=(const SMGCType& other) const;
};

}  // namespace smg
