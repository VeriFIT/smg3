#include <gtest/gtest.h>
#include <string>
#include <typeinfo>
#include "exceptions/UnsupportedOperationException.hh"
#include "objects/SMGRegion.hh"

namespace smg {

static int SIZE8 = 8;
static int SIZE10 = 10;

SMGRegion region_8 = SMGRegion(8, "region");

TEST(SMGRegion, TestToString) { EXPECT_FALSE(region_8.ToString().find('@') != std::string::npos); }

TEST(SMGRegion, IsAbstract) { EXPECT_FALSE(region_8.IsAbstract()); }

TEST(SMGRegion, TestJoin) {
  SMGRegion region_same(SIZE8, "region");
  const SMGObject& object_joint = region_8.Join(region_same);
  EXPECT_TRUE(typeid(object_joint) == typeid(SMGRegion));
  const SMGRegion& region_joint = dynamic_cast<const SMGRegion&>(object_joint);

  EXPECT_TRUE(region_joint.GetSize() == SIZE8);
  EXPECT_TRUE(region_joint.GetLabel() == "region");
}

TEST(SMGRegion, TestJoinDiffSize) {
  SMGRegion region_diff(SIZE10, "region");
  ASSERT_THROW(region_8.Join(region_diff), UnsupportedOperationException);
}

TEST(SMGRegion, TestPropertiesEqual) {
  SMGRegion one(SIZE8, "region");
  SMGRegion two(SIZE8, "region");
  SMGRegion three(SIZE10, "region");
  SMGRegion four(SIZE8, "REGION");

  EXPECT_TRUE(one.PropertiesEqual(one));
  EXPECT_TRUE(one.PropertiesEqual(two));
  EXPECT_FALSE(one.PropertiesEqual(three));
  EXPECT_FALSE(one.PropertiesEqual(four));
}

TEST(SMGRegion, TestIsMoreGeneral) { EXPECT_FALSE(region_8.IsMoreGeneral(region_8)); }

}  // namespace smg
