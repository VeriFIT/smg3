#pragma once

namespace smg {

class SMGCType {
 private:
  static const int SIZE_INT = 4;
  static const int SIZE_POINTER = 8;
  const int size_;

 public:
  static const SMGCType CreateTypeWithSize(const int size);
  static const SMGCType GetIntType();
  static const SMGCType GetPointerType();

  explicit SMGCType(const int size);
  int GetSize() const;
};

}  // namespace smg
