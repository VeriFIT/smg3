/*
    Created by Viktor Malik on 23.2.2016.
*/

#include <gtest/gtest.h>
#include "exceptions/IllegalArgumentException.hh"
#include "exceptions/NoSuchElementException.hh"
#include "graphs/CLangStackFrame.hh"

namespace smg {

const int SIZE8 = 8;
const int SIZE16 = 16;

class CLangStackFrameTest : public testing::Test {
 protected:
  CLangStackFrame sf;
};

TEST_F(CLangStackFrameTest, Constructor) {
  // Normal constructor
  const std::map<std::string, SMGRegionPtr> variables = sf.GetVariables();
  EXPECT_EQ(0, variables.size());
  EXPECT_FALSE(sf.ContainsVariable("foo"));

  // Copy constructor
  CLangStackFrame sf_copy(sf);
  EXPECT_EQ(0, sf_copy.GetVariables().size());
  EXPECT_FALSE(sf_copy.ContainsVariable("foo"));
}

TEST_F(CLangStackFrameTest, AddVariableTest) {
  sf.AddStackVariable("foo_var", std::make_shared<SMGRegion>(SIZE8, "foo_var_object"));
  EXPECT_TRUE(sf.ContainsVariable("foo_var"));

  const std::map<std::string, SMGRegionPtr> variables = sf.GetVariables();
  EXPECT_EQ(1, variables.size());
  SMGObjectPtr smg_object = variables.at("foo_var");
  EXPECT_EQ("foo_var_object", smg_object->GetLabel());
  EXPECT_EQ(SIZE8, smg_object->GetSize());

  smg_object.reset();
  smg_object = sf.GetVariable("foo_var");
  EXPECT_EQ("foo_var_object", smg_object->GetLabel());
  EXPECT_EQ(SIZE8, smg_object->GetSize());
}

TEST_F(CLangStackFrameTest, CLangStackFrameGetObjects) {
  EXPECT_EQ(0, sf.GetAllObjects().size());
  sf.AddStackVariable("foo_var", std::make_shared<SMGRegion>(SIZE8, "foo_var_object"));
  EXPECT_EQ(1, sf.GetAllObjects().size());
}

TEST_F(CLangStackFrameTest, CLangStackFrameAddVariableTwice) {
  sf.AddStackVariable("foo_var", std::make_shared<SMGRegion>(SIZE8, "foo_var_object"));
  SMGRegionPtr new_object = std::make_shared<SMGRegion>(SIZE16, "new_foo_var_object");
  EXPECT_THROW(sf.AddStackVariable("foo_var", new_object), IllegalArgumentException);
}

TEST_F(CLangStackFrameTest, CLangStackFrameMissingVariable) {
  EXPECT_FALSE(sf.ContainsVariable("foo_var"));
  EXPECT_THROW(sf.GetVariable("foo_var"), NoSuchElementException);
}

}  // namespace smg
