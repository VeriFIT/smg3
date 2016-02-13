#pragma once

#include <set>
#include <map>
#include <memory>

#include <SMGObject.hh>
#include "SMGValue.hh"
#include "SMGEdgeHasValue.hh"
#include "SMGEdgePointsTo.hh"

template<class T>
class SMGEntitySet {
private:
    std::set<std::shared_ptr<T>> entity_set;
public:
    typename std::set<std::shared_ptr<T>>::size_type size() const noexcept;
    bool contains(std::shared_ptr<T> pElement) const;
    bool empty() const noexcept;

    void add(const std::shared_ptr<T> pElement);
};

class SMG {
private:
    SMGEntitySet<const SMGObject> objects;
    std::set<SMGValue> values;
    std::map<SMGValue, SMGEdgePointsToPtr> pt_edges;
    SMGEntitySet<const SMGEdgeHasValue> hv_edges;

    SMGObjectPtr null_object = SMGNullObject::getNullObject();
    SMGValue null_value = SMGValue::getNullValue();

public:
    SMG();
    virtual ~SMG();

    void addObject(const SMGObjectPtr &pObject);
    void addValue(const SMGValue &pValue);
    void addPointsToEdge(const SMGEdgePointsToPtr &pEdge);
    void addHasValueEdge(const SMGEdgeHasValuePtr &pEdge);

    SMGObjectPtr getNullObject() const;
    const SMGValue getNullValue() const;

    const SMGEntitySet<const SMGObject>& getObjects() const;
    const std::set<SMGValue>& getValues() const;
    const std::map<SMGValue, SMGEdgePointsToPtr>& getPTEdges() const;
    const SMGEntitySet<const SMGEdgeHasValue>& getHVEdges() const;

    const SMGObjectPtr getObjectPointedBy(const SMGValue& pValue) const;
};

template<class T>
inline void SMGEntitySet<T>::add(std::shared_ptr<T> pElement) {
    entity_set.insert(pElement);
}

template<class T>
inline typename std::set<std::shared_ptr<T>>::size_type SMGEntitySet<T>::size() const noexcept {
    return entity_set.size();
}

template<class T>
inline bool SMGEntitySet<T>::contains(std::shared_ptr<T> pElement) const {
    return entity_set.count(pElement);
}

template<class T>
inline bool SMGEntitySet<T>::empty() const noexcept {
    return entity_set.empty();
}

