#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <boost\algorithm\string.hpp>

#include "graphs/SMG.hh"
#include <graphs/CLangSMG.hh>
#include <objects/SMGObject.hh>
namespace smg {

class SMGObjectNode {
private:
  const std::string name;
  const std::string definition;
  static int counter /*= 0 */;

public:
  SMGObjectNode(const std::string& pType, const std::string& pDefinition)
    : name{"node_" + pType + "_" + std::to_string(counter++)}, definition{pDefinition} {
  }

  SMGObjectNode(const std::string& pName) :name{pName}, definition{ } { }

  std::string getName() { return name; }

  std::string getDefinition() { return name + "[" + definition + "];"; }
};

//class SMGNodeDotVisitor /*: SMGObjectVisitor*/ {
//private: const SMG smg;
//private: SMGObjectNode node = null;
//
//public: SMGNodeDotVisitor(const SMG& pSmg) : smg{pSmg} { }
//
//private: std::string defaultDefinition(
//  const std::string& pColor,
//  const std::string& pShape,
//  const std::string& pStyle,
//  const SMGObject& pObject) {
//  return "color=" + pColor + ", shape=" + pShape + ", style=" + pStyle + ", label =\"" + /*pObject.ToString() +*/ "\"";
//}
//
//public: void visit(const SMGRegion pRegion) {
//           std::string shape = "rectangle";
//           std::string color;
//           std::string style;
//           if (smg.isObjectValid(pRegion)) {
//             color = "black"; style = "solid";
//           } else {
//             color = "red"; style = "dotted";
//           }
//
//           node = new SMGObjectNode("region", defaultDefinition(color, shape, style, pRegion));
//         }
//
//         
//public: void visit(const SMGSingleLinkedList pSll) {
//           std::string shape = "rectangle";
//           std::string color = "blue";
//
//           std::string style = "dashed";
//           node = new SMGObjectNode("sll", defaultDefinition(color, shape, style, pSll));
//         }
//
//         
//public: void visit(const SimpleBinaryTree pTree) {
//           std::string shape = "rectangle";
//           std::string color = "green";
//
//           std::string style = "dashed";
//           node = new SMGObjectNode("tree", defaultDefinition(color, shape, style, pTree));
//         }
//
//         
//public: void visit(const SMGObject pObject) {
//           if (pObject.notNull()) {
//             pObject.accept(this);
//           } else {
//             node = new SMGObjectNode("NULL");
//           }
//         }
//
//public: SMGObjectNode getNode() {
//  return node;
//}
//};

using namespace boost::algorithm;

class SMGPlotter {
//public: static void debuggingPlot(const SMG& pSmg, const std::string& pId) {
//  SMGPlotter plotter = new SMGPlotter();
//  PrintWriter writer = new PrintWriter(pId + ".dot", "UTF-8");
//  writer.write(plotter.smgAsDot(pSmg, pId, pId));
//  writer.close();
//}

private: 
  const std::map<SMGObjectPtr, SMGObjectNode> objectIndex; //init
  static int nulls /* = 0*/;
  int offset = 0;

public: 
  SMGPlotter() { } /* utility class */

 static std::string convertToValidDot(const std::string original) {
  return original/*.replaceAll("[:]", "_")*/;
}

 std::string smgAsDot(const CLangSMG& smg, const std::string name, const std::string location) {
   std::stringstream sb;

   sb << "digraph gr_" << replace_all_copy(name, '-', '_') << "{\n";
   offset += 2;
   sb << newLineWithOffset("label = \"Location: " + replace_all_copy(location, "\"", "\\\"") + "\";");

   addStackSubgraph(smg, sb);

   SMGNodeDotVisitor visitor = new SMGNodeDotVisitor(smg);

   for (SMGObject heapObject : smg.getHeapObjects()) {
     if (!objectIndex.containsKey(heapObject)) {
       visitor.visit(heapObject);
       objectIndex.put(heapObject, visitor.getNode());
     }
     if (heapObject.notNull()) {
       sb << (newLineWithOffset(objectIndex.get(heapObject).getDefinition()));
     }
   }

   addGlobalObjectSubgraph(smg, sb);

   for (Integer value : smg.getValues()) {
     if (value != smg.getNullValue()) {
       SMGExplicitValue explicitValue = smg.getExplicit(SMGKnownSymValue.valueOf(value));
       std::string explicitValueString;
       if (explicitValue.isUnknown()) {
         explicitValueString = "";
       } else {
         explicitValueString = " : " + std::to_string(explicitValue.getAsLong());
       }
       sb << (newLineWithOffset(smgValueAsDot(value, explicitValuestd::string)));
     }
   }

   for (SMGEdgeHasValue edge : smg.getHVEdges()) {
     sb << (newLineWithOffset(smgHVEdgeAsDot(edge)));
   }

   for (SMGEdgePointsTo edge : smg.getPTEdges()) {
     if (edge.getValue() != smg.getNullValue()) {
       sb << (newLineWithOffset(smgPTEdgeAsDot(edge)));
     }
   }

   sb << ("}");

   return sb.str();
 }

private:
  
void addStackSubgraph(const CLangSMG& pSmg, std::stringstream& pSb) {
  pSb << (newLineWithOffset("subgraph cluster_stack {"));
  offset += 2;
  pSb<< (newLineWithOffset("label=\"Stack\";"));

  int i = pSmg.GetStackFrames().size();
  for (auto stackItem : pSmg.GetStackFrames()) {
    AddStackItemSubgraph(stackItem, pSb, i);
    i--;
  }
  offset -= 2;
  pSb << (newLineWithOffset("}"));
}

void AddStackItemSubgraph(const CLangStackFrame pStackFrame, std::stringstream& pSb, const int pIndex) {
  pSb << (newLineWithOffset("subgraph cluster_stack_" + pStackFrame.GetFunctionDeclaration()/*.getName()*/ + "{"));
  offset += 2;
  pSb << (newLineWithOffset("fontcolor=blue;"));
  pSb << (newLineWithOffset("label=\"#" + std::to_string(pIndex) + ": " + pStackFrame.GetFunctionDeclaration() +
    "\";"));

  std::map<const std::string, SMGRegionPtr> toPrint();

  toPrint.putAll(pStackFrame.getVariables());

  SMGRegion returnObject = pStackFrame.getReturnObject();
  if (returnObject != null) {
    toPrint.put(CLangStackFrame.RETVAL_LABEL, returnObject);
  }

  pSb << (newLineWithOffset(smgScopeFrameAsDot(toPrint, std::string.valueOf(pIndex))));

  offset -= 2;
  pSb << (newLineWithOffset("}"));

}
//
//std::string smgScopeFrameAsDot(const Map<std::string, SMGRegion> pNamespace, const std::string& pStructId) {
//  std::stringBuilder sb = new std::stringBuilder();
//  sb << ("struct" + pStructId + "[shape=record,label=\" ");
//
//  // I sooo wish for Python list comprehension here...
//  ArrayList<std::string> nodes = new ArrayList<>();
//  for (Entry<std::string, SMGRegion> entry : pNamespace.entrySet()) {
//    std::string key = entry.getKey();
//    SMGObject obj = entry.getValue();
//
//    if (key.equals("node")) {
//      // escape Node1
//      key = "node1";
//    }
//
//    nodes.add("<item_" + key + "> " + obj.tostd::string());
//    objectIndex.put(obj, new SMGObjectNode("struct" + pStructId + ":item_" + key));
//  }
//  sb << (Joiner.on(" | ").join(nodes));
//  sb << ("\"];\n");
//  return sb.tostd::string();
//}
//
//void addGlobalObjectSubgraph(const ReadableSMG pSmg, const std::stringBuilder pSb) {
//  if (pSmg.getGlobalObjects().size() > 0) {
//    pSb << (newLineWithOffset("subgraph cluster_global{"));
//    offset += 2;
//    pSb << (newLineWithOffset("label=\"Global objects\";"));
//    pSb << (newLineWithOffset(smgScopeFrameAsDot(pSmg.getGlobalObjects(), "global")));
//    offset -= 2;
//    pSb << (newLineWithOffset("}"));
//  }
//}
//
//static std::string newNullLabel() {
//  SMGPlotter.nulls += 1;
//  return "value_null_" + SMGPlotter.nulls;
//}
//
//std::string smgHVEdgeAsDot(const SMGEdgeHasValue pEdge) {
//  if (pEdge.getValue() == 0) {
//    std::string newNull = newNullLabel();
//    return newNull + "[shape=plaintext, label=\"NULL\"];" + objectIndex.get(pEdge.getObject()).getName() + " -> " +
//      newNull + "[label=\"[" + pEdge.getOffset() + "]\"];";
//  } else {
//    return objectIndex.get(pEdge.getObject()).getName() + " -> value_" + pEdge.getValue() + "[label=\"[" +
//      pEdge.getOffset() + "]\"];";
//  }
//}
//
//std::string smgPTEdgeAsDot(const SMGEdgePointsTo pEdge) {
//  return "value_" + pEdge.getValue() + " -> " + objectIndex.get(pEdge.getObject()).getName() + "[label=\"+" +
//    pEdge.getOffset() + "b\"];";
//}
//
//static std::string smgValueAsDot(const int pValue, const std::string& pExplicit) {
//  return "value_" + pValue + "[label=\"#" + pValue + pExplicit + "\"];";
//}
//         
//static std::string neqRelationAsDot(const Integer v1, const Integer v2) {
//           std::string targetNode;
//           std::string returnstd::string = "";
//           if (v2.equals(0)) {
//             targetNode = newNullLabel();
//             returnstd::string = targetNode + "[shape=plaintext, label=\"NULL\", fontcolor=\"red\"];\n";
//           } else {
//             targetNode = "value_" + v2;
//           }
//           return returnstd::string + "value_" + v1 + " -> " + targetNode + "[color=\"red\", fontcolor=\"red\", label=\"neq\"]";
//         }
//

std::string newLineWithOffset(const std::string& pLine) {
  return std::string(offset, ' ') + pLine + "\n";
}
};

}  // namespace smg