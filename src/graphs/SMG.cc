#include "SMG.hh"

SMG::SMG() {
    SMGEdgePointsToPtr null_pointer = std::make_shared < SMGEdgePointsTo > (null_value, null_object, 0);
    addObject (null_object);
    addValue (null_value);
    addPointsToEdge(null_pointer);
}

SMG::~SMG() {
}

void SMG::addObject(const SMGObjectPtr &pObject) {
    objects.add(pObject);
}

void SMG::addValue(const SMGValue &pValue) {
    values.insert(pValue);
}

void SMG::addPointsToEdge(const SMGEdgePointsToPtr &pEdge) {
    pt_edges[pEdge->getValue()] = pEdge;
}

void SMG::addHasValueEdge(const SMGEdgeHasValuePtr &pEdge) {
    hv_edges.add(pEdge);
}

SMGObjectPtr SMG::getNullObject() const {
    return null_object;
}

const SMGValue SMG::getNullValue() const {
    return null_value;
}

const SMGEntitySet<const SMGObject>& SMG::getObjects() const {
    return objects;
}

const std::set<SMGValue>& SMG::getValues() const {
    return values;
}

const std::map<SMGValue, SMGEdgePointsToPtr>& SMG::getPTEdges() const {
    return pt_edges;
}

const SMGEntitySet<const SMGEdgeHasValue>& SMG::getHVEdges() const {
    return hv_edges;
}

const SMGObjectPtr SMG::getObjectPointedBy(const SMGValue& pValue) const {
    SMGEdgePointsToPtr edge = pt_edges.find(pValue)->second;
    return edge->getObject();
}
