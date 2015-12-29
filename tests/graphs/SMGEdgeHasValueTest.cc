#include <gtest/gtest.h>

#include <SMGRegion.hh>
#include <SMGObject.hh>
#include <SMGValue.hh>
#include <SMGEdgeHasValue.hh>
#include <SMGCType.hh>

const int SIZE4 = 4;
const int SIZE8 = 8;

const int OFFSET0 = 0;
const int OFFSET4 = 4;

const SMGCType& MOCKTYPE4 = SMGCType::createTypeWithSize(4);

TEST(SMGEdgeHasValue, sanity) {
    const SMGObject& obj = SMGRegion(SIZE8, "object");
    const SMGValue& val = SMGValue::getNewValue();
    const SMGEdgeHasValue& hv = SMGEdgeHasValue(MOCKTYPE4, OFFSET4, obj, val);

    EXPECT_EQ(obj.getId(), hv.getObject().getId());
    EXPECT_EQ(OFFSET4, hv.getOffset());
    std::cout << &hv.getType() << std::endl;
    EXPECT_EQ(&MOCKTYPE4, &hv.getType());
    EXPECT_EQ(SIZE4, hv.getSizeInBytes());
}

TEST(SMGEdgeHasValue, isConsistentWith) {
    const SMGObject& obj1 = SMGRegion(SIZE8, "object");
    const SMGObject& obj2 = SMGRegion(SIZE8, "different object");
    const SMGValue& val1 = SMGValue::getNewValue();
    const SMGValue& val2 = SMGValue::getNewValue();

    const SMGEdgeHasValue& hv1 = SMGEdgeHasValue(MOCKTYPE4, OFFSET0, obj1, val1);
    const SMGEdgeHasValue& hv2 = SMGEdgeHasValue(MOCKTYPE4, OFFSET4, obj1, val2);
    const SMGEdgeHasValue& hv3 = SMGEdgeHasValue(MOCKTYPE4, OFFSET4, obj1, val1);
    const SMGEdgeHasValue& hv4 = SMGEdgeHasValue(MOCKTYPE4, OFFSET4, obj2, val1);

    EXPECT_TRUE(hv1.isConsistentWith(hv1));
    EXPECT_TRUE(hv1.isConsistentWith(hv2));
    EXPECT_TRUE(hv1.isConsistentWith(hv3));
    EXPECT_FALSE(hv2.isConsistentWith(hv3));
    EXPECT_TRUE(hv2.isConsistentWith(hv4));
}

//  @Test
//    public final void testOverlapsWith() {
//    SMGObject object = new SMGRegion(SIZE12, "object");
//    Integer value = SMGValueFactory.getNewValue();
//
//    SMGEdgeHasValue at0 = new SMGEdgeHasValue(MOCKTYPE4, 0, object, value);
//    SMGEdgeHasValue at2 = new SMGEdgeHasValue(MOCKTYPE4, 2, object, value);
//
//    final int offset6 = 6;
//    SMGEdgeHasValue at4 = new SMGEdgeHasValue(MOCKTYPE4, OFFSET4, object, value);
//    SMGEdgeHasValue at6 = new SMGEdgeHasValue(MOCKTYPE4, offset6, object, value);
//
//    Assert.assertTrue(at0.overlapsWith(at2));
//    Assert.assertTrue(at2.overlapsWith(at0));
//    Assert.assertTrue(at2.overlapsWith(at4));
//    Assert.assertTrue(at4.overlapsWith(at2));
//    Assert.assertTrue(at4.overlapsWith(at6));
//    Assert.assertTrue(at6.overlapsWith(at4));
//
//    Assert.assertTrue(at0.overlapsWith(at0));
//
//    Assert.assertFalse(at0.overlapsWith(at4));
//    Assert.assertFalse(at0.overlapsWith(at6));
//    Assert.assertFalse(at2.overlapsWith(at6));
//    Assert.assertFalse(at4.overlapsWith(at0));
//    Assert.assertFalse(at6.overlapsWith(at0));
//    Assert.assertFalse(at6.overlapsWith(at2));
//
//    SMGEdgeHasValue whole = new SMGEdgeHasValue(MOCKTYPE12, 0, object, value);
//    Assert.assertTrue(whole.overlapsWith(at4));
//    Assert.assertTrue(at4.overlapsWith(whole));
//  }

//public class SMGEdgeHasValueTest {
//
//  private static final int OFFSET0 = 0;
//
//  private static final int LENGTH12 = 12;
//
//    private static final int SIZE12 = 12;
//
//  private static final CType MOCKTYPE12 = CType.createTypeWithLength(LENGTH12);
//
//
//
//
//  @Test
//    public final void testIsCompatibleField() {
//    SMGObject first = new SMGRegion(SIZE12, "object-1");
//    SMGObject second = new SMGRegion(SIZE12, "object-2");
//    Integer value = SMGValueFactory.getNewValue();
//
//    SMGEdgeHasValue firstAt0 = new SMGEdgeHasValue(MOCKTYPE4, 0, first, value);
//    SMGEdgeHasValue firstAt2 = new SMGEdgeHasValue(MOCKTYPE4, 2, first, value);
//    SMGEdgeHasValue firstAt4 = new SMGEdgeHasValue(MOCKTYPE4, OFFSET4, first, value);
//    SMGEdgeHasValue first12at0 = new SMGEdgeHasValue(MOCKTYPE12, 0, first, value);
//
//    SMGEdgeHasValue secondAt0 = new SMGEdgeHasValue(MOCKTYPE4, 0, second, value);
//    SMGEdgeHasValue secondAt2 = new SMGEdgeHasValue(MOCKTYPE4, 2, second, value);
//    SMGEdgeHasValue secondAt4 = new SMGEdgeHasValue(MOCKTYPE4, OFFSET4, second, value);
//    SMGEdgeHasValue second12at0 = new SMGEdgeHasValue(MOCKTYPE12, 0, second, value);
//
//    Assert.assertTrue(firstAt0.isCompatibleField(firstAt0));
//    Assert.assertFalse(firstAt0.isCompatibleField(firstAt2));
//    Assert.assertFalse(firstAt0.isCompatibleField(firstAt4));
//    Assert.assertFalse(firstAt0.isCompatibleField(first12at0));
//    Assert.assertTrue(firstAt0.isCompatibleField(secondAt0));
//    Assert.assertFalse(firstAt0.isCompatibleField(secondAt2));
//    Assert.assertFalse(firstAt0.isCompatibleField(secondAt4));
//    Assert.assertFalse(firstAt0.isCompatibleField(second12at0));
//  }
//
//  @Test
//    public final void testIsCompatibleFieldOnSameObject() {
//    SMGObject first = new SMGRegion(SIZE12, "object-1");
//    SMGObject second = new SMGRegion(SIZE12, "object-2");
//    Integer value = SMGValueFactory.getNewValue();
//
//    final int offset2 = 2;
//    SMGEdgeHasValue firstAt0 = new SMGEdgeHasValue(MOCKTYPE4, OFFSET0, first, value);
//    SMGEdgeHasValue firstAt2 = new SMGEdgeHasValue(MOCKTYPE4, offset2, first, value);
//    SMGEdgeHasValue firstAt4 = new SMGEdgeHasValue(MOCKTYPE4, OFFSET4, first, value);
//    SMGEdgeHasValue first12At0 = new SMGEdgeHasValue(MOCKTYPE12, OFFSET0, first, value);
//
//    SMGEdgeHasValue secondAt0 = new SMGEdgeHasValue(MOCKTYPE4, OFFSET0, second, value);
//    SMGEdgeHasValue secondAt2 = new SMGEdgeHasValue(MOCKTYPE4, offset2, second, value);
//    SMGEdgeHasValue secondAt4 = new SMGEdgeHasValue(MOCKTYPE4, OFFSET4, second, value);
//    SMGEdgeHasValue second12at0 = new SMGEdgeHasValue(MOCKTYPE12, OFFSET0, second, value);
//
//    Assert.assertTrue(firstAt0.isCompatibleFieldOnSameObject(firstAt0));
//    Assert.assertFalse(firstAt0.isCompatibleFieldOnSameObject(firstAt2));
//    Assert.assertFalse(firstAt0.isCompatibleFieldOnSameObject(firstAt4));
//    Assert.assertFalse(firstAt0.isCompatibleFieldOnSameObject(first12At0));
//    Assert.assertFalse(firstAt0.isCompatibleFieldOnSameObject(secondAt0));
//    Assert.assertFalse(firstAt0.isCompatibleFieldOnSameObject(secondAt2));
//    Assert.assertFalse(firstAt0.isCompatibleFieldOnSameObject(secondAt4));
//    Assert.assertFalse(firstAt0.isCompatibleFieldOnSameObject(second12at0));
//  }
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
//    SMGEdgeHasValueFilter filter = SMGEdgeHasValueFilter.objectFilter(object1).filterHavingValue(value1);
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
