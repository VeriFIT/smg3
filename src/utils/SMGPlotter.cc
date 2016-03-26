#include "SMGPlotter.hh"
#include <iostream>

namespace smg {

int SMGObjectNode::counter = 0;
int SMGPlotter::nulls = 0;

std::string SMGNodeDotVisitor::defaultDefinition(
  const std::string & pColor,
  const std::string & pShape,
  const std::string & pStyle,
  const SMGObject & pObject) {
  return
    "color=" + pColor +
    ", shape=" + pShape +
    ", style=" + pStyle +
    ", label =\"" + pObject.ToString() + "\"";
}

void SMGNodeDotVisitor::Visit(const SMGObject & pObject) {
  if (pObject.NotNull()) {
    pObject.Accept(*this);
  } else {
    node = SMGObjectNode("NULL");
  }
}

void SMGNodeDotVisitor::Visit(const SMGRegion & pRegion) {
  std::string shape = "rectangle";
  std::string color;
  std::string style;
  if (smg.IsObjectValid(pRegion)) {
    color = "black"; style = "solid";
  } else {
    color = "red"; style = "dotted";
  }

  node = SMGObjectNode("region", defaultDefinition(color, shape, style, pRegion));
}

//void SMGNodeDotVisitor::visit(const SMGSingleLinkedList & pSll) {
//  std::string shape = "rectangle";
//  std::string color = "blue";
//
//  std::string style = "dashed";
//  node = SMGObjectNode("sll", defaultDefinition(color, shape, style, pSll));
//}
//
//void SMGNodeDotVisitor::visit(const SimpleBinaryTree & pTree) {
//  std::string shape = "rectangle";
//  std::string color = "green";
//
//  std::string style = "dashed";
//  node = SMGObjectNode("tree", defaultDefinition(color, shape, style, pTree));
//}

void SMGPlotter::debuggingPlot(const CLangSMG & pSmg, const std::string & pId) {
  SMGPlotter plotter = SMGPlotter();
  //std::ofstream writer(pId + ".dot", std::ios_base::out);
  std::ostream& writer = std::cout;
  writer << plotter.smgAsDot(pSmg, pId, pId);
  writer.flush();
  //writer.close();
}

std::string SMGPlotter::convertToValidDot(const std::string original) {
  return original/*.replaceAll("[:]", "_")*/;
}

std::string SMGPlotter::smgAsDot(const CLangSMG & smg, const std::string name, const std::string location) {
  std::stringstream sb;

  sb << "digraph gr_" << replace_all_copy(name, "-", "_") << "{\n";
  offset += 2;
  sb << newLineWithOffset("label = \"Location: " + replace_all_copy(location, "\"", "\\\"") + "\";");

  addStackSubgraph(smg, sb);

  SMGNodeDotVisitor visitor(smg);

  for (auto& heapObject : smg.GetHeapObjects()) {
    if (objectIndex.find(heapObject) != objectIndex.end()) {
      visitor.Visit(*heapObject);
      objectIndex.emplace(heapObject, visitor.getNode());
    }
    if ((*heapObject).NotNull()) {
      sb << (newLineWithOffset(objectIndex.at(heapObject).getDefinition()));
    }
  }

  addGlobalObjectSubgraph(smg, sb);

  for (auto& value : smg.GetValues()) {
    if (value != smg.GetNullValue()) {
      //SMGExplicitValue explicitValue = smg.GetExplicit(SMGKnownSymValue.valueOf(value));
      std::string explicitValueString;
      //if (explicitValue.isUnknown()) {
      explicitValueString = "";
      //} else {
      //  explicitValueString = " : " + std::to_string(explicitValue.getAsLong());
      //}
      sb << (newLineWithOffset(smgValueAsDot(value, explicitValueString)));
    }
  }

  for (const auto& edge : smg.GetHVEdges()) {
    sb << (newLineWithOffset(smgHVEdgeAsDot(*edge)));
  }

  for (const auto& edge : smg.GetPTEdges()) {
    if ((*edge.second).GetValue() != smg.GetNullValue()) {
      sb << (newLineWithOffset(smgPTEdgeAsDot(*edge.second)));
    }
  }

  sb << ("}");

  return sb.str();
}

void SMGPlotter::addStackSubgraph(const CLangSMG & pSmg, std::stringstream & pSb) {
  pSb << (newLineWithOffset("subgraph cluster_stack {"));
  offset += 2;
  pSb << (newLineWithOffset("label=\"Stack\";"));

  size_t i = pSmg.GetStackFrames().size();
  for (auto stackItem : pSmg.GetStackFrames()) {
    AddStackItemSubgraph(stackItem, pSb, i);
    i--;
  }
  offset -= 2;
  pSb << (newLineWithOffset("}"));
}

void SMGPlotter::AddStackItemSubgraph(const CLangStackFrame & pStackFrame, std::stringstream & pSb, const size_t pIndex) {
  pSb << (newLineWithOffset("subgraph cluster_stack_" + pStackFrame.GetFunctionDeclaration()/*.getName()*/ + "{"));
  offset += 2;
  pSb << (newLineWithOffset("fontcolor=blue;"));
  pSb << (newLineWithOffset("label=\"#" + std::to_string(pIndex) + ": " + pStackFrame.GetFunctionDeclaration() +
    "\";"));

  std::map<std::string, SMGRegionPtr> toPrint(pStackFrame.GetVariables());

  SMGRegionPtr returnObject = pStackFrame.GetReturnObject();

  //if (returnObject != null) {
  toPrint.emplace(/*CLangStackFrame::RETVAL_LABEL*/ "___cpa_temp_result_var_", returnObject);
  //}

  pSb << (newLineWithOffset(smgScopeFrameAsDot(toPrint, std::to_string(pIndex))));

  offset -= 2;
  pSb << (newLineWithOffset("}"));

}

std::string SMGPlotter::smgScopeFrameAsDot(const std::map<std::string, SMGRegionPtr>& pNamespace, const std::string & pStructId) {
  std::stringstream sb(std::ios_base::out);
  sb << "struct" << pStructId + "[shape=record,label=\" ";

  // I sooo wish for Python list comprehension here...
  std::vector<std::string> nodes{ };

  for (auto entry : pNamespace) {
    std::string key = entry.first;
    SMGObjectPtr obj = entry.second;

    if (key == "node") {
      // escape Node1
      key = "node1";
    }

    nodes.push_back("<item_" + key + "> " + (*obj).ToString());
    objectIndex.emplace(obj, SMGObjectNode("struct" + pStructId + ":item_" + key));
  }
  sb << (join(nodes, "|"));
  sb << ("\"];\n");
  return sb.str();
}

void SMGPlotter::addGlobalObjectSubgraph(const CLangSMG & pSmg, std::stringstream & pSb) {
  if (pSmg.GetGlobalVariables().size() > 0) {
    pSb << (newLineWithOffset("subgraph cluster_global{"));
    offset += 2;
    pSb << (newLineWithOffset("label=\"Global objects\";"));
    pSb << (newLineWithOffset(smgScopeFrameAsDot(pSmg.GetGlobalVariables(), "global")));
    offset -= 2;
    pSb << (newLineWithOffset("}"));
  }
}

std::string SMGPlotter::newNullLabel() {
  nulls += 1;
  return "value_null_" + nulls;
}

std::string SMGPlotter::smgHVEdgeAsDot(const SMGEdgeHasValue & edge) {
  if (edge.GetValue() == SMGValue::GetNullValue()) {
    std::string newNull = newNullLabel();
    return
      newNull + "[shape=plaintext, label=\"NULL\"];" +
      objectIndex.at(edge.GetObject()).getName() + " -> " +
      newNull + "[label=\"[" + std::to_string(edge.GetOffset()) + "]\"];";
  } else {
    return
      objectIndex.at(edge.GetObject()).getName() +
      " -> value_" + std::to_string(edge.GetValue().GetId()) +
      "[label=\"[" + std::to_string(edge.GetOffset()) + "]\"];";
  }
}

std::string SMGPlotter::smgPTEdgeAsDot(const SMGEdgePointsTo & edge) {
  return
    "value_" + std::to_string(edge.GetValue().GetId()) + " -> " +
    objectIndex.at(edge.GetObject()).getName() +
    "[label=\"+" + std::to_string(edge.GetOffset()) + "b\"];";
}

std::string SMGPlotter::smgValueAsDot(const SMGValue & pValue, const std::string & pExplicit) {
  return
    "value_" + std::to_string(pValue.GetId()) +
    "[label=\"#" + std::to_string(pValue.GetId()) + pExplicit + "\"];";
}

std::string SMGPlotter::neqRelationAsDot(const SMGValue & v1, const SMGValue & v2) {
  std::string targetNode;
  std::string returnString = "";
  if (v2 == v2.GetNullValue()) {
    targetNode = newNullLabel();
    returnString = targetNode + "[shape=plaintext, label=\"NULL\", fontcolor=\"red\"];\n";
  } else {
    targetNode = "value_" + std::to_string(v2.GetId());
  }
  return returnString + "value_" + std::to_string(v1.GetId()) + " -> " + targetNode + "[color=\"red\", fontcolor=\"red\", label=\"neq\"]";
}

std::string SMGPlotter::newLineWithOffset(const std::string & pLine) {
  return std::string(offset, ' ') + pLine + "\n";
}

}