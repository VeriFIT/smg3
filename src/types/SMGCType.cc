#include "SMGCType.hh"

namespace smg {

const SMGCType SMGCType::CreateTypeWithSize(const int size) { return SMGCType(size); }

const SMGCType SMGCType::GetIntType() { return CreateTypeWithSize(SMGCType::SIZE_INT); }

const SMGCType SMGCType::GetPointerType() { return CreateTypeWithSize(SMGCType::SIZE_POINTER); }

const SMGCType SMGCType::GetInvalidType() { return CreateTypeWithSize(SMGCType::SIZE_INVALID); }

SMGCType::SMGCType(const int size) : size_(size) { }

int SMGCType::GetSize() const { return size_; }

bool SMGCType::operator==(const SMGCType& other) const {
  return size_ == other.size_;
}

bool SMGCType::operator!=(const SMGCType& other) const {
  return size_ != other.size_;
}

}  // namespace smg
