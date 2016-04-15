#include <iostream>
#include <memory>
#include "exceptions/IllegalArgumentException.hh"
#include "graphs/CLangSMG.hh"
#include "graphs/CLangSMGConsistencyVerifier.hh"
#include "utils/SMGPlotter.hh"

using namespace smg;

const int SIZE8 = 8;
const int SIZE16 = 16;
const int SIZE32 = 32;
const int OFFSET0 = 0;

const SMGCType mock_type = SMGCType::CreateTypeWithSize(0);
const SMGCType TYPE8 = SMGCType::CreateTypeWithSize(SIZE8);
const SMGCType TYPE16 = SMGCType::CreateTypeWithSize(SIZE16);
const SMGCType TYPE32 = SMGCType::CreateTypeWithSize(SIZE32);

const std::string function_declaration = "foo";

int main() {
  CLangSMG smg;

  SMGRegionPtr obj_1 = std::make_shared<SMGRegion>(SIZE8, "obj_1");
  SMGRegionPtr obj_2 = smg.AddGlobalVariable(TYPE8, "obj_2");
  SMGValue val_1 = SMGValue::GetNewValue();
  SMGValue val_2 = SMGValue::GetNewValue();
  SMGEdgePointsToPtr pt = std::make_shared<SMGEdgePointsTo>(val_1, obj_1, OFFSET0);
  SMGEdgeHasValuePtr
      hv = std::make_shared<SMGEdgeHasValue>(SMGCType::GetIntType(), OFFSET0, obj_2, val_2);

  smg.AddValue(val_1);
  smg.AddValue(val_2);
  smg.AddHeapObject(obj_1);
  smg.AddPointsToEdge(pt);
  smg.AddHasValueEdge(hv);

  std::string dot = SMGPlotter::PlotToString(smg, "Moje_smg", "moje_lokace");
  std::cout << dot;

  return 0;
}

  // namespace smg
