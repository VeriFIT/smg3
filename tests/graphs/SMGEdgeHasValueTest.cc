#include <gtest/gtest.h>
#include "graphs/SMGEdgeHasValue.hh"
#include "graphs/SMGValue.hh"
#include "objects/SMGObject.hh"
#include "objects/SMGRegion.hh"
#include "types/SMGCType.hh"

namespace smg {

const int SIZE4 = 4;
const int SIZE8 = 8;
const int SIZE12 = 12;

const int OFFSET0 = 0;
const int OFFSET2 = 2;
const int OFFSET4 = 4;
const int OFFSET6 = 6;

const SMGCType MOCKTYPE4 = TypeImpl::create(1,4,32);
const SMGCType MOCKTYPE12 = TypeImpl::create(2,12,80);

TEST(SMGEdgeHasValue, Sanity) {
  const SMGObject& obj = SMGRegion(SIZE8, "object");
  const SMGValue& val = SMGValue::GetNewValue();
  const SMGEdgeHasValue& hv = SMGEdgeHasValue(MOCKTYPE4, OFFSET4, obj, val);

  EXPECT_EQ(obj.GetId(), hv.GetObject().GetId());
  EXPECT_EQ(OFFSET4, hv.GetOffset());
  EXPECT_EQ(&MOCKTYPE4, &hv.GetType());
  EXPECT_EQ(SIZE4, hv.GetSizeInBytes());
}

TEST(SMGEdgeHasValue, IsConsistentWith) {
  const SMGObject& obj_1 = SMGRegion(SIZE8, "object");
  const SMGObject& obj_2 = SMGRegion(SIZE8, "different object");
  const SMGValue& val_1 = SMGValue::GetNewValue();
  const SMGValue& val_2 = SMGValue::GetNewValue();

  const SMGEdgeHasValue& hv_1 = SMGEdgeHasValue(MOCKTYPE4, OFFSET0, obj_1, val_1);
  const SMGEdgeHasValue& hv_2 = SMGEdgeHasValue(MOCKTYPE4, OFFSET4, obj_1, val_2);
  const SMGEdgeHasValue& hv_3 = SMGEdgeHasValue(MOCKTYPE4, OFFSET4, obj_1, val_1);
  const SMGEdgeHasValue& hv_4 = SMGEdgeHasValue(MOCKTYPE4, OFFSET4, obj_2, val_1);

  EXPECT_TRUE(hv_1.IsConsistentWith(hv_1));
  EXPECT_TRUE(hv_1.IsConsistentWith(hv_2));
  EXPECT_TRUE(hv_1.IsConsistentWith(hv_3));
  EXPECT_FALSE(hv_2.IsConsistentWith(hv_3));
  EXPECT_TRUE(hv_2.IsConsistentWith(hv_4));
}

TEST(SMGEdgeHasValue, OverlapsWith) {
  const SMGObject& object = SMGRegion(SIZE12, "object");
  const SMGValue& value = SMGValue(SMGValue::GetNewValue());

  const SMGEdgeHasValue& at_0 = SMGEdgeHasValue(MOCKTYPE4, OFFSET0, object, value);
  const SMGEdgeHasValue& at_2 = SMGEdgeHasValue(MOCKTYPE4, OFFSET2, object, value);
  const SMGEdgeHasValue& at_4 = SMGEdgeHasValue(MOCKTYPE4, OFFSET4, object, value);
  const SMGEdgeHasValue& at_6 = SMGEdgeHasValue(MOCKTYPE4, OFFSET6, object, value);

  EXPECT_TRUE(at_0.OverlapsWith(at_2));
  EXPECT_TRUE(at_2.OverlapsWith(at_0));
  EXPECT_TRUE(at_2.OverlapsWith(at_4));
  EXPECT_TRUE(at_4.OverlapsWith(at_2));
  EXPECT_TRUE(at_4.OverlapsWith(at_6));
  EXPECT_TRUE(at_6.OverlapsWith(at_4));

  EXPECT_TRUE(at_0.OverlapsWith(at_0));

  EXPECT_FALSE(at_0.OverlapsWith(at_4));
  EXPECT_FALSE(at_0.OverlapsWith(at_6));
  EXPECT_FALSE(at_2.OverlapsWith(at_6));
  EXPECT_FALSE(at_4.OverlapsWith(at_0));
  EXPECT_FALSE(at_6.OverlapsWith(at_0));
  EXPECT_FALSE(at_6.OverlapsWith(at_2));

  const SMGEdgeHasValue& whole = SMGEdgeHasValue(MOCKTYPE12, OFFSET0, object, value);
  EXPECT_TRUE(whole.OverlapsWith(at_4));
  EXPECT_TRUE(at_4.OverlapsWith(whole));
}

TEST(SMGEdgeHasValue, IsCompatibleField) {
  const SMGObject& first = SMGRegion(SIZE12, "object-1");
  const SMGObject& second = SMGRegion(SIZE12, "object-2");
  const SMGValue& value = SMGValue::GetNewValue();

  const SMGEdgeHasValue& first_at_0 = SMGEdgeHasValue(MOCKTYPE4, OFFSET0, first, value);
  const SMGEdgeHasValue& first_at_2 = SMGEdgeHasValue(MOCKTYPE4, OFFSET2, first, value);
  const SMGEdgeHasValue& first_at_4 = SMGEdgeHasValue(MOCKTYPE4, OFFSET4, first, value);
  const SMGEdgeHasValue& first_12_at_0 = SMGEdgeHasValue(MOCKTYPE12, OFFSET0, first, value);

  const SMGEdgeHasValue& second_at_0 = SMGEdgeHasValue(MOCKTYPE4, OFFSET0, second, value);
  const SMGEdgeHasValue& second_at_2 = SMGEdgeHasValue(MOCKTYPE4, OFFSET2, second, value);
  const SMGEdgeHasValue& second_at_4 = SMGEdgeHasValue(MOCKTYPE4, OFFSET4, second, value);
  const SMGEdgeHasValue& second_12_at_0 = SMGEdgeHasValue(MOCKTYPE12, OFFSET0, second, value);

  EXPECT_TRUE(first_at_0.IsCompatibleField(first_at_0));
  EXPECT_FALSE(first_at_0.IsCompatibleField(first_at_2));
  EXPECT_FALSE(first_at_0.IsCompatibleField(first_at_4));
  EXPECT_FALSE(first_at_0.IsCompatibleField(first_12_at_0));

  EXPECT_TRUE(first_at_0.IsCompatibleField(second_at_0));
  EXPECT_FALSE(first_at_0.IsCompatibleField(second_at_2));
  EXPECT_FALSE(first_at_0.IsCompatibleField(second_at_4));
  EXPECT_FALSE(first_at_0.IsCompatibleField(second_12_at_0));
}

TEST(SMGEdgeHasValue, IsCompatibleFieldOnSameObject) {
  const SMGObject& first = SMGRegion(SIZE12, "object-1");
  const SMGObject& second = SMGRegion(SIZE12, "object-2");
  const SMGValue& value = SMGValue::GetNewValue();

  const SMGEdgeHasValue first_at_0 = SMGEdgeHasValue(MOCKTYPE4, OFFSET0, first, value);
  const SMGEdgeHasValue first_at_2 = SMGEdgeHasValue(MOCKTYPE4, OFFSET2, first, value);
  const SMGEdgeHasValue first_at_4 = SMGEdgeHasValue(MOCKTYPE4, OFFSET4, first, value);
  const SMGEdgeHasValue first_12_at_0 = SMGEdgeHasValue(MOCKTYPE12, OFFSET0, first, value);

  const SMGEdgeHasValue second_at_0 = SMGEdgeHasValue(MOCKTYPE4, OFFSET0, second, value);
  const SMGEdgeHasValue second_at_2 = SMGEdgeHasValue(MOCKTYPE4, OFFSET2, second, value);
  const SMGEdgeHasValue second_at_4 = SMGEdgeHasValue(MOCKTYPE4, OFFSET4, second, value);
  const SMGEdgeHasValue second_12_at_0 = SMGEdgeHasValue(MOCKTYPE12, OFFSET0, second, value);

  EXPECT_TRUE(first_at_0.IsCompatibleFieldOnSameObject(first_at_0));
  EXPECT_FALSE(first_at_0.IsCompatibleFieldOnSameObject(first_at_2));
  EXPECT_FALSE(first_at_0.IsCompatibleFieldOnSameObject(first_at_4));
  EXPECT_FALSE(first_at_0.IsCompatibleFieldOnSameObject(first_12_at_0));
  EXPECT_FALSE(first_at_0.IsCompatibleFieldOnSameObject(second_at_0));
  EXPECT_FALSE(first_at_0.IsCompatibleFieldOnSameObject(second_at_2));
  EXPECT_FALSE(first_at_0.IsCompatibleFieldOnSameObject(second_at_4));
  EXPECT_FALSE(first_at_0.IsCompatibleFieldOnSameObject(second_12_at_0));
}
//
//  @Test(expected = IllegalArgumentException.class)
//    public final void testIllegalOverlapsWith() {
//    SMGObject object1 = new SMGRegion(SIZE12, "object1");
//    SMGObject object2 = new SMGRegion(SIZE12, "object2");
//    Integer value = SMGValueFactory.getNewValue();
//
//    SMGEdgeHasValue hv1 = new SMGEdgeHasValue(MOCKTYPE4, 0, object1, value);
//    SMGEdgeHasValue hv2 = new SMGEdgeHasValue(MOCKTYPE4, 2, object2, value);
//
//    hv1.overlapsWith(hv2);
//  }
//
//  @Test
//    public final void testFilterAsPredicate() {
//    SMGObject object1 = new SMGRegion(SIZE8, "object1");
//
//    Integer value1 = Integer.valueOf(1);
//    Integer value2 = Integer.valueOf(2);
//
//    SMGEdgeHasValue hv11at0 = new SMGEdgeHasValue(MOCKTYPE4, 0, object1, value1);
//    SMGEdgeHasValue hv12at0 = new SMGEdgeHasValue(MOCKTYPE4, 0, object1, value2);
//
//    SMGEdgeHasValueFilter filter =
// SMGEdgeHasValueFilter.objectFilter(object1).filterHavingValue(value1);
//    Predicate<SMGEdgeHasValue> predicate = filter.asPredicate();
//
//    Assert.assertTrue(predicate.apply(hv11at0));
//    Assert.assertFalse(predicate.apply(hv12at0));
//  }
//
//  @Test
//    public final void testFilterOnObject() {
//    SMGObject object1 = new SMGRegion(SIZE8, "object1");
//    SMGObject object2 = new SMGRegion(SIZE8, "Object2");
//
//    Integer value1 = Integer.valueOf(1);
//    Integer value2 = Integer.valueOf(2);
//
//    SMGEdgeHasValue hv11at0 = new SMGEdgeHasValue(MOCKTYPE4, 0, object1, value1);
//    SMGEdgeHasValue hv12at0 = new SMGEdgeHasValue(MOCKTYPE4, 0, object1, value2);
//    SMGEdgeHasValue hv21at0 = new SMGEdgeHasValue(MOCKTYPE4, 0, object2, value1);
//    SMGEdgeHasValue hv22at0 = new SMGEdgeHasValue(MOCKTYPE4, 0, object2, value2);
//
//    Set<SMGEdgeHasValue> allEdges = new HashSet<>();
//    allEdges.add(hv11at0);
//    allEdges.add(hv12at0);
//    allEdges.add(hv21at0);
//    allEdges.add(hv22at0);
//
//    SMGEdgeHasValueFilter filter = new SMGEdgeHasValueFilter();
//
//    Assert.assertTrue(filter.holdsFor(hv11at0));
//    Assert.assertTrue(filter.holdsFor(hv12at0));
//    Assert.assertTrue(filter.holdsFor(hv21at0));
//    Assert.assertTrue(filter.holdsFor(hv22at0));
//
//    filter.filterByObject(object1);
//
//    Assert.assertTrue(filter.holdsFor(hv11at0));
//    Assert.assertTrue(filter.holdsFor(hv12at0));
//    Assert.assertFalse(filter.holdsFor(hv21at0));
//    Assert.assertFalse(filter.holdsFor(hv22at0));
//
//    Set<SMGEdgeHasValue> filteredSet = filter.filterSet(allEdges);
//
//    Assert.assertEquals(2, filteredSet.size());
//    Assert.assertTrue(filteredSet.contains(hv11at0));
//    Assert.assertTrue(filteredSet.contains(hv12at0));
//  }
//
//  @Test
//    public final void testFilterAtOffset() {
//    SMGObject object1 = new SMGRegion(SIZE8, "object1");
//    SMGObject object2 = new SMGRegion(SIZE8, "Object2");
//
//    Integer value1 = Integer.valueOf(1);
//    Integer value2 = Integer.valueOf(2);
//
//    SMGEdgeHasValue hv11at0 = new SMGEdgeHasValue(MOCKTYPE4, OFFSET0, object1, value1);
//    SMGEdgeHasValue hv12at0 = new SMGEdgeHasValue(MOCKTYPE4, OFFSET4, object1, value2);
//    SMGEdgeHasValue hv21at0 = new SMGEdgeHasValue(MOCKTYPE4, OFFSET0, object2, value1);
//    SMGEdgeHasValue hv22at0 = new SMGEdgeHasValue(MOCKTYPE4, OFFSET4, object2, value2);
//    Set<SMGEdgeHasValue> allEdges = new HashSet<>();
//    allEdges.add(hv11at0);
//    allEdges.add(hv12at0);
//    allEdges.add(hv21at0);
//    allEdges.add(hv22at0);
//
//    SMGEdgeHasValueFilter filter = new SMGEdgeHasValueFilter();
//
//    filter.filterAtOffset(0);
//
//    Assert.assertTrue(filter.holdsFor(hv11at0));
//    Assert.assertFalse(filter.holdsFor(hv12at0));
//    Assert.assertTrue(filter.holdsFor(hv21at0));
//    Assert.assertFalse(filter.holdsFor(hv22at0));
//
//    Set<SMGEdgeHasValue> filteredSet = filter.filterSet(allEdges);
//
//    Assert.assertEquals(2, filteredSet.size());
//    Assert.assertTrue(filteredSet.contains(hv11at0));
//    Assert.assertTrue(filteredSet.contains(hv21at0));
//  }
//
//  @Test
//    public final void testFilterOnValue() {
//    SMGObject object1 = new SMGRegion(SIZE8, "object1");
//    SMGObject object2 = new SMGRegion(SIZE8, "Object2");
//
//    Integer value1 = Integer.valueOf(1);
//    Integer value2 = Integer.valueOf(2);
//
//    SMGEdgeHasValue hv11at0 = new SMGEdgeHasValue(MOCKTYPE4, OFFSET0, object1, value1);
//    SMGEdgeHasValue hv12at0 = new SMGEdgeHasValue(MOCKTYPE4, OFFSET4, object1, value2);
//    SMGEdgeHasValue hv21at0 = new SMGEdgeHasValue(MOCKTYPE4, OFFSET0, object2, value1);
//    SMGEdgeHasValue hv22at0 = new SMGEdgeHasValue(MOCKTYPE4, OFFSET4, object2, value2);
//    Set<SMGEdgeHasValue> allEdges = new HashSet<>();
//    allEdges.add(hv11at0);
//    allEdges.add(hv12at0);
//    allEdges.add(hv21at0);
//    allEdges.add(hv22at0);
//
//    SMGEdgeHasValueFilter filter = new SMGEdgeHasValueFilter();
//
//    filter.filterHavingValue(value1);
//
//    Assert.assertTrue(filter.holdsFor(hv11at0));
//    Assert.assertFalse(filter.holdsFor(hv12at0));
//    Assert.assertTrue(filter.holdsFor(hv21at0));
//    Assert.assertFalse(filter.holdsFor(hv22at0));
//
//    Set<SMGEdgeHasValue> filteredSet = filter.filterSet(allEdges);
//
//    Assert.assertEquals(2, filteredSet.size());
//    Assert.assertTrue(filteredSet.contains(hv11at0));
//    Assert.assertTrue(filteredSet.contains(hv21at0));
//
//    filter.filterNotHavingValue(value1);
//
//    Assert.assertFalse(filter.holdsFor(hv11at0));
//    Assert.assertTrue(filter.holdsFor(hv12at0));
//    Assert.assertFalse(filter.holdsFor(hv21at0));
//    Assert.assertTrue(filter.holdsFor(hv22at0));
//
//    filteredSet = filter.filterSet(allEdges);
//
//    Assert.assertEquals(2, filteredSet.size());
//    Assert.assertTrue(filteredSet.contains(hv22at0));
//    Assert.assertTrue(filteredSet.contains(hv12at0));
//  }

}  // namespace smg
