/*
    Created by Viktor Malik on 24.2.2016.
*/

#include <gtest/gtest.h>
#include <memory>
#include "graphs/CLangSMG.hh"
#include "graphs/CLangSMGConsistencyVerifier.hh"

namespace smg {

const int SIZE8 = 8;
const int OFFSET0 = 0;

const SMGCType mock_type = SMGCType::CreateTypeWithSize(0);

const std::string function_declaration = "foo";

TEST(CLangSMGConsistencyVerifier, ConsistencyViolationDisjunctness1) {
  CLangSMG smg;

  EXPECT_TRUE(CLangSMGConsistencyVerifier::Verify(smg));
  SMGRegionPtr obj = std::make_shared<SMGRegion>(SIZE8, "label");
  smg.AddGlobalObject(obj);
  EXPECT_TRUE(CLangSMGConsistencyVerifier::Verify(smg));
  smg.AddHeapObject(obj);
  EXPECT_FALSE(CLangSMGConsistencyVerifier::Verify(smg));
}

TEST(CLangSMGConsistencyVerifier, ConsistencyViolationDisjunctness2) {
  CLangSMG smg;
  smg.AddStackFrame(function_declaration);

  EXPECT_TRUE(CLangSMGConsistencyVerifier::Verify(smg));
  SMGRegionPtr obj = std::make_shared<SMGRegion>(SIZE8, "label");
  smg.AddStackObject(obj);
  EXPECT_TRUE(CLangSMGConsistencyVerifier::Verify(smg));
  smg.AddHeapObject(obj);
  EXPECT_FALSE(CLangSMGConsistencyVerifier::Verify(smg));
}

TEST(CLangSMGConsistencyVerifier, ConsistencyViolationUnion) {
  CLangSMG smg;
  EXPECT_TRUE(CLangSMGConsistencyVerifier::Verify(smg));
  SMGRegionPtr heap_obj = std::make_shared<SMGRegion>(SIZE8, "heap_object");
  SMGRegionPtr dummy_obj = std::make_shared<SMGRegion>(SIZE8, "dummy_object");

  smg.AddStackFrame(function_declaration);
  EXPECT_TRUE(CLangSMGConsistencyVerifier::Verify(smg));
  smg.AddStackObject(std::make_shared<SMGRegion>(SIZE8, "stack_variable"));
  EXPECT_TRUE(CLangSMGConsistencyVerifier::Verify(smg));
  smg.AddGlobalObject(std::make_shared<SMGRegion>(SIZE8, "global_variable"));
  EXPECT_TRUE(CLangSMGConsistencyVerifier::Verify(smg));
  smg.AddHeapObject(heap_obj);
  EXPECT_TRUE(CLangSMGConsistencyVerifier::Verify(smg));
  smg.AddObject(dummy_obj);
  EXPECT_FALSE(CLangSMGConsistencyVerifier::Verify(smg));
}

TEST(CLangSMGConsistencyVerifier, ConsistencyViolationNull) {
  CLangSMG smg;
  SMGObjectPtr null_object = *(smg.GetHeapObjects().begin());
  SMGValue value = SMGValue::GetNewValue();
  SMGEdgeHasValuePtr
      edge = std::make_shared<SMGEdgeHasValue>(mock_type, OFFSET0, null_object, value);

  smg.AddValue(value);
  smg.AddHasValueEdge(edge);
  EXPECT_FALSE(CLangSMGConsistencyVerifier::Verify(smg));
}

}  // namespace smg
