#pragma once

#include <set>
#include <map>
#include <memory>
#include "graphs/SMGEdgeHasValue.hh"
#include "graphs/SMGEdgeHasValueFilter.hh"
#include "graphs/SMGEdgePointsTo.hh"
#include "graphs/SMGValue.hh"
#include "objects/SMGObject.hh"

namespace smg {

template<class T> class SMGEntitySet {
 private:
  std::set<std::shared_ptr<T>> entity_set;

 public:
  typename std::set<std::shared_ptr<T>>::size_type size() const noexcept;
  bool contains(std::shared_ptr<T> element) const;
  bool empty() const noexcept;
  std::set<std::shared_ptr<T>>& set();

  void add(const std::shared_ptr<T> element);
  void remove(const std::shared_ptr<T> element);
  typename std::set<std::shared_ptr<T>>::iterator
      remove(typename std::set<std::shared_ptr<T>>::iterator position);

  typename std::set<std::shared_ptr<T>>::const_iterator begin() const noexcept;
  typename std::set<std::shared_ptr<T>>::const_iterator end() const noexcept;
  typename std::set<std::shared_ptr<T>>::iterator begin() noexcept;
  typename std::set<std::shared_ptr<T>>::iterator end() noexcept;
};

class SMG {
 private:
  SMGEntitySet<const SMGObject> objects_;
  std::set<SMGValue> values_;
  std::map<SMGValue, SMGEdgePointsToPtr> pt_edges_;
  SMGEntitySet<const SMGEdgeHasValue> hv_edges_;
  std::map<long, bool> object_validity_;

  SMGObjectPtr null_object_ = SMGNullObject::GetNullObject();
  SMGValue null_value_ = SMGValue::GetNullValue();

 public:
  SMG();
  virtual ~SMG();

  void AddObject(const SMGObjectPtr& object);
  void AddObject(const SMGObjectPtr& object, const bool validity);
  void AddValue(const SMGValue& value);
  void AddPointsToEdge(const SMGEdgePointsToPtr& edge);
  void AddHasValueEdge(const SMGEdgeHasValuePtr& edge);
  void RemoveObject(const SMGObjectPtr& object);
  void RemoveValue(const SMGValue& value);
  void RemovePointsToEdge(const SMGEdgePointsToPtr& edge);
  void RemoveHasValueEdge(const SMGEdgeHasValuePtr& edge);
  void RemoveObjectAndEdges(const SMGObjectPtr& object);

  void SetValidity(const SMGObjectPtr& object, const bool validity);
  bool IsObjectValid(const SMGObjectPtr& object) const;

  SMGObjectPtr GetNullObject() const;
  const SMGValue GetNullValue() const;

  const SMGEntitySet<const SMGObject>& GetObjects() const;
  const std::set<SMGValue>& GetValues() const;
  const std::map<SMGValue, SMGEdgePointsToPtr>& GetPTEdges() const;
  const SMGEntitySet<const SMGEdgeHasValue>& GetHVEdges() const;
  const SMGEntitySet<const SMGEdgeHasValue> GetHVEdges(const SMGEdgeHasValueFilter& filter) const;
  const SMGEdgeHasValuePtr GetUniqueHV(const SMGEdgeHasValueFilter& filter, const bool check);

  const SMGObjectPtr GetObjectPointedBy(const SMGValue& value) const;

  std::vector<bool> GetNullBytesForObject(const SMGObjectPtr& obj) const;
};

template<class T> inline void SMGEntitySet<T>::add(std::shared_ptr<T> element) {
  entity_set.insert(element);
}

template<class T> inline void SMGEntitySet<T>::remove(const std::shared_ptr<T> element) {
  entity_set.erase(element);
}

template<class T>
inline typename std::set<std::shared_ptr<T>>::iterator SMGEntitySet<T>::remove(
    typename std::set<std::shared_ptr<T>>::iterator position) {
  return entity_set.erase(position);
}

template<class T>
inline typename std::set<std::shared_ptr<T>>::size_type SMGEntitySet<T>::size() const noexcept {
  return entity_set.size();
}

template<class T> inline bool SMGEntitySet<T>::contains(std::shared_ptr<T> element) const {
  return entity_set.find(element) != entity_set.end();
}

template<class T> inline bool SMGEntitySet<T>::empty() const noexcept {
  return entity_set.empty();
}

template<class T>
inline typename std::set<std::shared_ptr<T>>::const_iterator SMGEntitySet<T>::begin() const
noexcept {
  return entity_set.begin();
}

template<class T>
inline typename std::set<std::shared_ptr<T>>::const_iterator SMGEntitySet<T>::end() const noexcept {
  return entity_set.end();
}

template<class T>
inline typename std::set<std::shared_ptr<T>>::iterator SMGEntitySet<T>::begin() noexcept {
  return entity_set.begin();
}

template<class T>
inline typename std::set<std::shared_ptr<T>>::iterator SMGEntitySet<T>::end() noexcept {
  return entity_set.end();
}

template<class T> inline std::set<std::shared_ptr<T>>& SMGEntitySet<T>::set() {
  return entity_set;
}

}  // namespace smg
