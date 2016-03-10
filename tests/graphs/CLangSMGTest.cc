/*
    Created by Viktor Malik on 24.2.2016.
*/

#include <gtest/gtest.h>
#include <memory>
#include "exceptions/IllegalArgumentException.hh"
#include "graphs/CLangSMG.hh"
#include "graphs/CLangSMGConsistencyVerifier.hh"

namespace smg {

const int SIZE8 = 8;
const int SIZE16 = 16;
const int SIZE32 = 32;
const int OFFSET0 = 0;

const SMGCType mock_type = SMGCType::CreateTypeWithSize(0);
const SMGCType TYPE8 = SMGCType::CreateTypeWithSize(SIZE8);
const SMGCType TYPE16 = SMGCType::CreateTypeWithSize(SIZE16);
const SMGCType TYPE32 = SMGCType::CreateTypeWithSize(SIZE32);

const std::string function_declaration = "foo";

TEST(CLangSMG, Constructor) {
  CLangSMG smg;

  EXPECT_EQ(0, smg.GetStackFrames().size());
  EXPECT_EQ(1, smg.GetHeapObjects().size());
  EXPECT_EQ(0, smg.GetGlobalVariables().size());

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
  EXPECT_TRUE(CLangSMGConsistencyVerifier::Verify(smg));

  // Copy constructor

  CLangSMG smg_copy(smg);
  EXPECT_TRUE(CLangSMGConsistencyVerifier::Verify(smg_copy));

  EXPECT_EQ(0, smg_copy.GetStackFrames().size());
  EXPECT_EQ(2, smg_copy.GetHeapObjects().size());
  EXPECT_EQ(1, smg_copy.GetGlobalVariables().size());

  EXPECT_EQ(obj_1, smg_copy.GetObjectPointedBy(val_1));

  SMGEdgeHasValueFilter filter = SMGEdgeHasValueFilter::ObjectFilter(obj_2);
  EXPECT_EQ(hv, smg_copy.GetUniqueHV(filter, true));
}

TEST(CLangSMG, AddHeapObject) {
  CLangSMG smg;
  SMGRegionPtr obj_1 = std::make_shared<SMGRegion>(SIZE8, "label");
  SMGRegionPtr obj_2 = std::make_shared<SMGRegion>(SIZE8, "label");

  smg.AddHeapObject(obj_1);
  EXPECT_TRUE(CLangSMGConsistencyVerifier::Verify(smg));
  std::set<SMGObjectPtr> heap_objs = smg.GetHeapObjects();

  EXPECT_TRUE(heap_objs.find(obj_1) != heap_objs.end());
  EXPECT_FALSE(heap_objs.find(obj_2) != heap_objs.end());
  EXPECT_EQ(2, heap_objs.size());

  smg.AddHeapObject(obj_2);
  EXPECT_TRUE(CLangSMGConsistencyVerifier::Verify(smg));
  heap_objs = smg.GetHeapObjects();

  EXPECT_TRUE(heap_objs.find(obj_1) != heap_objs.end());
  EXPECT_TRUE(heap_objs.find(obj_2) != heap_objs.end());
  EXPECT_EQ(3, heap_objs.size());
}

TEST(CLangSMG, AddHeapObjectTwice) {
  CLangSMG::SetPerformChecks(true);
  CLangSMG smg;
  SMGRegionPtr obj = std::make_shared<SMGRegion>(SIZE8, "label");
  smg.AddHeapObject(obj);
  EXPECT_THROW(smg.AddHeapObject(obj), IllegalArgumentException);
}

TEST(CLangSMG, AddHeapObjectTwiceWithoutTest) {
  CLangSMG::SetPerformChecks(false);
  CLangSMG smg;
  SMGRegionPtr obj = std::make_shared<SMGRegion>(SIZE8, "label");
  smg.AddHeapObject(obj);
  EXPECT_NO_THROW(smg.AddHeapObject(obj));
}

TEST(CLangSMG, AddGlobalObject) {
  CLangSMG smg;
  SMGRegionPtr obj_1 = smg.AddGlobalVariable(TYPE8, "label");

  auto global_objects = smg.GetGlobalObjects();

  EXPECT_TRUE(CLangSMGConsistencyVerifier::Verify(smg));
  EXPECT_EQ(1, global_objects.size());
  EXPECT_TRUE(global_objects.find(obj_1) != global_objects.end());

  SMGRegionPtr obj_2 = smg.AddGlobalVariable(TYPE8, "another_label");
  global_objects = smg.GetGlobalObjects();

  EXPECT_TRUE(CLangSMGConsistencyVerifier::Verify(smg));
  EXPECT_EQ(2, global_objects.size());
  EXPECT_TRUE(global_objects.find(obj_1) != global_objects.end());
  EXPECT_TRUE(global_objects.find(obj_2) != global_objects.end());
}

TEST(CLangSMG, AddGlobalObjectTwice) {
  CLangSMG::SetPerformChecks(true);
  CLangSMG smg;
  smg.AddGlobalVariable(TYPE8, "label");
  EXPECT_THROW(smg.AddGlobalVariable(TYPE8, "label"), IllegalArgumentException);
}

TEST(CLangSMG, AddGlobalObjectWithSameLabel) {
  CLangSMG::SetPerformChecks(true);
  CLangSMG smg;
  smg.AddGlobalVariable(TYPE8, "label");
  EXPECT_THROW(smg.AddGlobalVariable(TYPE16, "label"), IllegalArgumentException);
}

TEST(ClangSMG, AddStackObject) {
  CLangSMG smg;
  smg.AddStackFrame(function_declaration);

  SMGRegionPtr obj = smg.AddLocalVariable(TYPE8, "label");
  CLangStackFrame current_frame = smg.GetStackFrames().front();

  EXPECT_TRUE(CLangSMGConsistencyVerifier::Verify(smg));
  EXPECT_EQ(obj, current_frame.GetVariable("label"));
  EXPECT_EQ(1, current_frame.GetVariables().size());

  SMGRegionPtr diff_obj = smg.AddLocalVariable(TYPE8, "diff_label");
  current_frame = smg.GetStackFrames().front();

  EXPECT_TRUE(CLangSMGConsistencyVerifier::Verify(smg));
  EXPECT_EQ(obj, current_frame.GetVariable("label"));
  EXPECT_EQ(diff_obj, current_frame.GetVariable("diff_label"));
  EXPECT_EQ(2, current_frame.GetVariables().size());

  EXPECT_TRUE(smg.HasLocalVariable("label"));
  EXPECT_TRUE(smg.HasLocalVariable("diff_label"));
}

TEST(ClangSMG, AddStackObjectTwice) {
  CLangSMG::SetPerformChecks(true);
  CLangSMG smg;
  smg.AddStackFrame(function_declaration);
  smg.AddLocalVariable(TYPE8, "label");
  EXPECT_THROW(smg.AddLocalVariable(TYPE8, "label"), IllegalArgumentException);
}

TEST(CLangSMG, GetObjectForVisibleVariable) {
  CLangSMG smg;

  SMGRegionPtr obj_3 = smg.AddGlobalVariable(TYPE32, "label");
  EXPECT_EQ(obj_3, smg.GetObjectForVisibleVariable("label"));

  smg.AddStackFrame(function_declaration);
  SMGRegionPtr obj_1 = smg.AddLocalVariable(TYPE8, "label");
  EXPECT_EQ(obj_1, smg.GetObjectForVisibleVariable("label"));

  smg.AddStackFrame(function_declaration);
  SMGRegionPtr obj_2 = smg.AddLocalVariable(TYPE16, "label");
  EXPECT_EQ(obj_2, smg.GetObjectForVisibleVariable("label"));
  EXPECT_NE(obj_1, smg.GetObjectForVisibleVariable("label"));

  smg.AddStackFrame(function_declaration);
  EXPECT_EQ(obj_3, smg.GetObjectForVisibleVariable("label"));
  EXPECT_NE(obj_2, smg.GetObjectForVisibleVariable("label"));
}

TEST(CLangSMG, GetStackFrames) {
  CLangSMG smg;
  EXPECT_EQ(0, smg.GetStackFrames().size());

  smg.AddStackFrame(function_declaration);
  smg.AddLocalVariable(TYPE8, "frame_1_1");
  smg.AddLocalVariable(TYPE8, "frame_1_2");
  smg.AddLocalVariable(TYPE8, "frame_1_3");
  EXPECT_EQ(1, smg.GetStackFrames().size());
  EXPECT_EQ(3, smg.GetStackFrames().front().GetVariables().size());

  smg.AddStackFrame(function_declaration);
  smg.AddLocalVariable(TYPE8, "frame_2_1");
  smg.AddLocalVariable(TYPE8, "frame_2_2");
  EXPECT_EQ(2, smg.GetStackFrames().size());
  EXPECT_EQ(2, smg.GetStackFrames().front().GetVariables().size());

  smg.AddStackFrame(function_declaration);
  smg.AddLocalVariable(TYPE8, "frame_3_1");
  EXPECT_EQ(3, smg.GetStackFrames().size());
  EXPECT_EQ(1, smg.GetStackFrames().front().GetVariables().size());

  smg.AddStackFrame(function_declaration);
  EXPECT_EQ(4, smg.GetStackFrames().size());
  EXPECT_EQ(0, smg.GetStackFrames().front().GetVariables().size());
}

TEST(CLangSMG, GetHeapObjects) {
  CLangSMG smg;
  EXPECT_EQ(1, smg.GetHeapObjects().size());

  smg.AddHeapObject(std::make_shared<SMGRegion>(SIZE8, "heap_1"));
  EXPECT_EQ(2, smg.GetHeapObjects().size());

  smg.AddHeapObject(std::make_shared<SMGRegion>(SIZE8, "heap_2"));
  smg.AddHeapObject(std::make_shared<SMGRegion>(SIZE8, "heap_3"));
  EXPECT_EQ(4, smg.GetHeapObjects().size());
}

TEST(CLangSMG, GetGlobalObjects) {
  CLangSMG smg;
  EXPECT_EQ(0, smg.GetGlobalVariables().size());

  smg.AddGlobalVariable(TYPE8, "heap_1");
  EXPECT_EQ(1, smg.GetGlobalObjects().size());

  smg.AddGlobalVariable(TYPE8, "heap_2");
  smg.AddGlobalVariable(TYPE8, "heap_3");
  EXPECT_EQ(3, smg.GetGlobalVariables().size());
}

TEST(ClangSMG, MemoryLeaks) {
  CLangSMG smg;
  EXPECT_FALSE(smg.HasMemoryLeaks());
  smg.SetMemoryLeak();
  EXPECT_TRUE(smg.HasMemoryLeaks());
}

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

TEST(CLangSMGConsistencyVerifier, ConsistencyViolationStackNamespace2) {
  CLangSMG smg;
  smg.AddStackFrame(function_declaration);
  smg.AddLocalVariable(TYPE8, "label");
  smg.AddStackFrame(function_declaration);
  smg.AddLocalVariable(TYPE16, "label");

  EXPECT_TRUE(CLangSMGConsistencyVerifier::Verify(smg));
}

TEST(CLangSMGConsistencyVerifier, ConsistencyViolationStackNamespace3) {
  CLangSMG smg;
  smg.AddGlobalVariable(TYPE8, "label");
  smg.AddStackFrame(function_declaration);
  smg.AddLocalVariable(TYPE16, "label");
  EXPECT_TRUE(CLangSMGConsistencyVerifier::Verify(smg));
}

}  // namespace smg
