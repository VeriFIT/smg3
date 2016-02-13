#include "gtest/gtest.h"
#include <SMGRegion.hh>
#include <UnsupportedOperationException.hh>
#include <string>
#include <typeinfo>

static int SIZE8 = 8;
static int SIZE10 = 10;

SMGRegion region8 = SMGRegion(8, "region");

TEST(SMGRegion, testToString) {
    EXPECT_FALSE(region8.toString().find('@') != std::string::npos);
}

TEST(SMGRegion, isAbstract) {
    EXPECT_FALSE(region8.isAbstract());
}

TEST(SMGRegion, testPropertiesEqual) {
    SMGRegion one(SIZE8, "region");
    SMGRegion two(SIZE8, "region");
    SMGRegion three(SIZE10, "region");
    SMGRegion four(SIZE8, "REGION");

    EXPECT_TRUE(one.propertiesEqual(one));
    EXPECT_TRUE(one.propertiesEqual(two));
    EXPECT_FALSE(one.propertiesEqual(three));
    EXPECT_FALSE(one.propertiesEqual(four));
}

TEST(SMGRegion, testIsMoreGeneral) {
    EXPECT_FALSE(region8.isMoreGeneral(region8));
}
