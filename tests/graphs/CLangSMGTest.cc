/*
    Created by Viktor Malik on 24.2.2016.
*/

#include <gtest/gtest.h>
#include <memory>
#include "graphs/CLangSMG.hh"
#include "graphs/CLangSMGConsistencyVerifier.hh"

namespace smg{

const int SIZE8 = 8;

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

}