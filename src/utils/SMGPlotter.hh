#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <map>
#include <boost/algorithm/string.hpp>

#include "graphs/SMG.hh"
#include "graphs/CLangSMG.hh"
#include "objects/SMGObject.hh"
#include "exceptions/UnsupportedOperationException.hh"
#include "objects/SMGObjectVisitor.hh"
namespace smg {

class SMGObjectNode {
private:
  /*const*/ std::string name;
  /*const*/ std::string definition;
  static int counter;

public:
  SMGObjectNode() = default;
  //SMGObjectNode& operator=(const SMGObjectNode&) = default;

  SMGObjectNode(const std::string& pType, const std::string& pDefinition)
    : name{"node_" + pType + "_" + std::to_string(counter++)}, definition{pDefinition} {
  }

  explicit SMGObjectNode(const std::string& pName) :name{pName}, definition{ } { }

  std::string getName() { return name; }

  std::string getDefinition() { return name + "[" + definition + "];"; }
};

class SMGNodeDotVisitor : public SMGObjectVisitor {
private:
  const SMG& smg;
  SMGObjectNode node;

public:
  explicit SMGNodeDotVisitor(const SMG& pSmg) : smg{pSmg} { }

private:
  std::string defaultDefinition(
    const std::string& pColor,
    const std::string& pShape,
    const std::string& pStyle,
    const SMGObject& pObject);

public:
  void Visit(const SMGObject& pObject) override;
  void Visit(const SMGRegion& pRegion) override;
  //virtual void visit(const SMGSingleLinkedList& pSll);
  //virtual void visit(const SimpleBinaryTree& pTree);

  SMGObjectNode getNode() {
    return node;
  }
};

using namespace boost::algorithm;

class SMGPlotter {
private:
  /*const*/ std::map<SMGObjectPtr, SMGObjectNode> objectIndex; //init
  static int nulls;
  uint32_t offset = 0;

public:
  //SMGPlotter() { } /* utility class */

  static void debuggingPlot(const CLangSMG& pSmg, const std::string& pId);

  static std::string convertToValidDot(const std::string original);

  std::string smgAsDot(const CLangSMG& smg, const std::string name, const std::string location);

private:
  void addStackSubgraph(const CLangSMG& pSmg, std::stringstream& pSb);

  void AddStackItemSubgraph(
    const CLangStackFrame& pStackFrame, std::stringstream& pSb, const size_t pIndex);

  std::string smgScopeFrameAsDot(
    const std::map<std::string, SMGRegionPtr>& pNamespace, const std::string& pStructId);

  void addGlobalObjectSubgraph(const CLangSMG& pSmg, std::stringstream& pSb);

  static std::string newNullLabel();

  std::string smgHVEdgeAsDot(const SMGEdgeHasValue& edge);

  std::string smgPTEdgeAsDot(const SMGEdgePointsTo& edge);

  static std::string smgValueAsDot(const SMGValue& pValue, const std::string& pExplicit);

  static std::string neqRelationAsDot(const SMGValue& v1, const SMGValue& v2);

  std::string newLineWithOffset(const std::string& pLine);
};

}  // namespace smg
