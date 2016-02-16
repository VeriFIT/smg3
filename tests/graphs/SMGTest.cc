/*
 *  SMGTest.cc
 *
 *  This file is part of SMG, a symbolic memory graph library
 *
 *  Copyright (C) 2016  Petr Muller
 *  All rights reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#include <gtest/gtest.h>
#include <SMGEdgePointsTo.hh>
#include <SMGEdgeHasValue.hh>
#include <SMGValue.hh>
#include <SMGObject.hh>
#include <SMGRegion.hh>
#include <SMGCType.hh>
#include <SMG.hh>
#include <SMGConsistencyVerifier.hh>

const int SIZE4 = 4;
const int SIZE8 = 8;
const int SIZE16 = 16;

const int OFFSET0 = 0;
const int OFFSET4 = 4;

const SMGCType& mockType = SMGCType::createTypeWithSize(SIZE4);

// obj1 = xxxxxxxx
// obj2 = yyyyzzzz
// val1 -> obj1
// yyyy has value 2
// zzzz has value 1

const SMGObjectPtr obj1 = std::make_shared < SMGRegion > (SIZE8, "object-1");
const SMGObjectPtr obj2 = std::make_shared < SMGRegion > (SIZE8, "object-2");

const SMGValue val1 = SMGValue::getNewValue();
const SMGValue val2 = SMGValue::getNewValue();

const SMGEdgePointsToPtr pt1to1 = std::make_shared < SMGEdgePointsTo > (val1, obj1, OFFSET0);
const SMGEdgeHasValuePtr hv2has2at0 = std::make_shared < SMGEdgeHasValue > (mockType, OFFSET0, obj2, val2);
const SMGEdgeHasValuePtr hv2has1at4 = std::make_shared < SMGEdgeHasValue > (mockType, OFFSET4, obj2, val1);

class SMGTest: public testing::Test {
protected:
    SMG smg = SMG();
    SMG emptySmg = SMG();

    virtual void setUp() {
        smg.addObject(obj1);
        smg.addObject(obj2);

        smg.addValue(val1);
        smg.addValue(val2);

        smg.addPointsToEdge(pt1to1);

        smg.addHasValueEdge(hv2has2at0);
        smg.addHasValueEdge(hv2has1at4);
    }
};

TEST_F(SMGTest, constructorTest) {
    EXPECT_TRUE(SMGConsistencyVerifier::verify(emptySmg));
    const SMGObjectPtr nullObject = emptySmg.getNullObject();
    const SMGValue nullAddress = emptySmg.getNullValue();

    EXPECT_FALSE(nullObject->notNull());
    EXPECT_EQ(1, emptySmg.getObjects().size());
    EXPECT_TRUE(emptySmg.getObjects().contains(nullObject));

    EXPECT_EQ(1, emptySmg.getValues().size());
    EXPECT_TRUE(emptySmg.getValues().count(nullAddress));

    EXPECT_EQ(1, emptySmg.getPTEdges().size());
    const SMGObjectPtr targetObject = emptySmg.getObjectPointedBy(nullAddress);
    EXPECT_EQ(nullObject->getId(), targetObject->getId());

    EXPECT_TRUE(emptySmg.getHVEdges().empty());

    // COPY CONSTRUCTOR

    SMG smgCopy = SMG(emptySmg);

    EXPECT_TRUE(SMGConsistencyVerifier::verify(emptySmg));
    EXPECT_TRUE(SMGConsistencyVerifier::verify(smgCopy));

    const SMGObjectPtr thirdObject = std::make_shared<SMGRegion>(SIZE16, "object-3");
    const SMGValue thirdValue = SMGValue::getNewValue();
    smgCopy.addObject(thirdObject);
    smgCopy.addValue(thirdValue);
    smgCopy.addHasValueEdge(std::make_shared<SMGEdgeHasValue>(mockType, OFFSET0, thirdObject, thirdValue));
    smgCopy.addPointsToEdge(std::make_shared<SMGEdgePointsTo>(thirdValue, thirdObject, OFFSET0));

    EXPECT_TRUE(SMGConsistencyVerifier::verify(emptySmg));
    EXPECT_TRUE(SMGConsistencyVerifier::verify(smgCopy));
    EXPECT_EQ(1, emptySmg.getObjects().size());
    EXPECT_EQ(2, smgCopy.getObjects().size());
    EXPECT_EQ(1, smgCopy.getObjects().contains(thirdObject));

    EXPECT_EQ(1, emptySmg.getValues().size());
    EXPECT_EQ(2, smgCopy.getValues().size());
    EXPECT_EQ(1, smgCopy.getValues().count(thirdValue));

    EXPECT_EQ(1, emptySmg.getPTEdges().size());
    EXPECT_EQ(2, smgCopy.getPTEdges().size());
    const SMGObjectPtr targetObjectForThird = smgCopy.getObjectPointedBy(thirdValue);
    EXPECT_EQ(thirdObject.get(), targetObjectForThird.get());

    EXPECT_TRUE(emptySmg.getHVEdges().empty());
    EXPECT_EQ(1, smgCopy.getHVEdges().size());
}

//
//  @Test(expected = NoSuchElementException.class)
//  public final void getUniqueHV0Test() {
//    SMGEdgeHasValueFilter filter = SMGEdgeHasValueFilter.objectFilter(obj1);
//    smg.getUniqueHV(filter, false);
//  }
//
//  @Test
//  public final void getUniqueHV1Test() {
//    SMGEdgeHasValueFilter filter = SMGEdgeHasValueFilter.objectFilter(obj2).filterAtOffset(0);
//    Assert.assertEquals(smg.getUniqueHV(filter, true), hv2has2at0);
//    Assert.assertEquals(smg.getUniqueHV(filter, false), hv2has2at0);
//  }
//
//  @Test(expected = IllegalArgumentException.class)
//  public final void getUniqueHV2CheckTest() {
//    SMGEdgeHasValueFilter filter = SMGEdgeHasValueFilter.objectFilter(obj2);
//    smg.getUniqueHV(filter, true);
//  }
//
//  @Test(expected = IllegalArgumentException.class)
//  public final void getUniqueHV2NoCheckTest() {
//    SMGEdgeHasValueFilter filter = SMGEdgeHasValueFilter.objectFilter(obj2);
//    Assert.assertNotNull(smg.getUniqueHV(filter, true));
//  }
//
//  @Test
//  public final void getNullBytesForObjectTest() {
//    emptySmg.addObject(obj1);
//    final int offset4 = 4;
//    SMGEdgeHasValue hv = new SMGEdgeHasValue(mockType, offset4, obj1, emptySmg.getNullValue());
//    emptySmg.addHasValueEdge(hv);
//
//    final int offset3 = 3;
//    final int offset7 = 7;
//    BitSet bs = emptySmg.getNullBytesForObject(obj1);
//    Assert.assertFalse(bs.get(0));
//    Assert.assertFalse(bs.get(offset3));
//    Assert.assertTrue(bs.get(offset4));
//    Assert.assertTrue(bs.get(offset7));
//  }
//
//  @Test
//  public final void replaceHVSetTest() {
//    SMGEdgeHasValue hv = new SMGEdgeHasValue(mockType, 2, obj1, val1.intValue());
//    Set<SMGEdgeHasValue> hvSet = new HashSet<>();
//    hvSet.add(hv);
//
//    smg.replaceHVSet(hvSet);
//    Set<SMGEdgeHasValue> newHVSet = Sets.newHashSet(smg.getHVEdges());
//    Assert.assertTrue(hvSet.equals(newHVSet));
//  }
//
//  @Test
//  public final void constructorTest() {
//  }
//
//  @Test
//  public final void addRemoveHasValueEdgeTest() {
//    SMGObject object = new SMGRegion(SIZE4, "object");
//
//    SMGEdgeHasValue hv = new SMGEdgeHasValue(mockType, 0, object, emptySmg.getNullValue());
//
//    emptySmg.addHasValueEdge(hv);
//    Assert.assertTrue(Iterables.contains(emptySmg.getHVEdges(), hv));
//
//    emptySmg.removeHasValueEdge(hv);
//    Assert.assertFalse(Iterables.contains(emptySmg.getHVEdges(), hv));
//  }
//
//  @Test
//  public final void removeObjectTest() {
//    Integer newValue = SMGValueFactory.getNewValue();
//
//    SMGObject object = new SMGRegion(SIZE8, "object");
//    SMGEdgeHasValue hv0 = new SMGEdgeHasValue(mockType, OFFSET0, object, 0);
//    SMGEdgeHasValue hv4 = new SMGEdgeHasValue(mockType, OFFSET4, object, 0);
//    SMGEdgePointsTo pt = new SMGEdgePointsTo(newValue, object, 0);
//
//    emptySmg.addValue(newValue);
//    emptySmg.addObject(object);
//    emptySmg.addPointsToEdge(pt);
//    emptySmg.addHasValueEdge(hv0);
//    emptySmg.addHasValueEdge(hv4);
//
//    Assert.assertTrue(emptySmg.getObjects().contains(object));
//    emptySmg.removeObject(object);
//    Assert.assertFalse(emptySmg.getObjects().contains(object));
//    Assert.assertTrue(emptySmg.getPTEdges().contains(pt));
//
//    Assert.assertTrue(Iterables.contains(emptySmg.getHVEdges(), hv0));
//    Assert.assertTrue(Iterables.contains(emptySmg.getHVEdges(), hv4));
//  }
//
//  @Test
//  public final void removeObjectAndEdgesTest() {
//    Integer newValue = SMGValueFactory.getNewValue();
//
//    SMGObject object = new SMGRegion(SIZE8, "object");
//    SMGEdgeHasValue hv0 = new SMGEdgeHasValue(mockType, OFFSET0, object, 0);
//    SMGEdgeHasValue hv4 = new SMGEdgeHasValue(mockType, OFFSET4, object, 0);
//    SMGEdgePointsTo pt = new SMGEdgePointsTo(newValue, object, 0);
//
//    emptySmg.addValue(newValue);
//    emptySmg.addObject(object);
//    emptySmg.addPointsToEdge(pt);
//    emptySmg.addHasValueEdge(hv0);
//    emptySmg.addHasValueEdge(hv4);
//
//    Assert.assertTrue(emptySmg.getObjects().contains(object));
//    emptySmg.removeObjectAndEdges(object);
//    Assert.assertFalse(emptySmg.getObjects().contains(object));
//    Assert.assertFalse(emptySmg.getPTEdges().contains(pt));
//    Assert.assertFalse(Iterables.contains(emptySmg.getHVEdges(), hv0));
//    Assert.assertFalse(Iterables.contains(emptySmg.getHVEdges(), hv4));
//  }
//
//  @Test
//  public final void validityTest() {
//    Assert.assertFalse(smg.isObjectValid(smg.getNullObject()));
//    Assert.assertTrue(smg.isObjectValid(obj1));
//    Assert.assertTrue(smg.isObjectValid(obj2));
//
//    SMG smgCopy = new SMG(smg);
//    Assert.assertTrue(SMGConsistencyVerifier.verifySMG(smgCopy));
//    Assert.assertTrue(SMGConsistencyVerifier.verifySMG(smg));
//
//    smg.setValidity(obj1, false);
//    Assert.assertTrue(SMGConsistencyVerifier.verifySMG(smgCopy));
//    Assert.assertTrue(SMGConsistencyVerifier.verifySMG(smg));
//    Assert.assertFalse(smg.isObjectValid(smg.getNullObject()));
//    Assert.assertFalse(smg.isObjectValid(obj1));
//    Assert.assertTrue(smg.isObjectValid(obj2));
//    Assert.assertFalse(smgCopy.isObjectValid(smgCopy.getNullObject()));
//    Assert.assertTrue(smgCopy.isObjectValid(obj1));
//    Assert.assertTrue(smgCopy.isObjectValid(obj2));
//
//    smg.setValidity(obj2, false);
//    Assert.assertTrue(SMGConsistencyVerifier.verifySMG(smgCopy));
//    Assert.assertFalse(smgCopy.isObjectValid(smgCopy.getNullObject()));
//    Assert.assertTrue(smgCopy.isObjectValid(obj1));
//    Assert.assertTrue(smgCopy.isObjectValid(obj2));
//  }
//
//  @Test
//  public final void consistencyViolationInvalidRegionHasValueTest() {
//    smg.setValidity(obj1, false);
//    Assert.assertTrue(SMGConsistencyVerifier.verifySMG(smg));
//    smg.setValidity(obj2, false);
//    Assert.assertFalse(SMGConsistencyVerifier.verifySMG(smg));
//  }
//
//  @Test
//  public final void consistencyViolationFieldConsistency() {
//    SMG smg1 = getNewSMG64();
//    SMG smg2 = getNewSMG64();
//
//    SMGObject object2b = new SMGRegion(SIZE2, "object_2b");
//    SMGObject object4b = new SMGRegion(SIZE4, "object_4b");
//    Integer value = SMGValueFactory.getNewValue();
//
//    smg1.addObject(object2b);
//    smg2.addObject(object4b);
//    smg1.addValue(value);
//    smg2.addValue(value);
//
//    // Read 4 bytes (sizeof(mockType)) on offset 0 of 2b object -> out of bounds
//    SMGEdgeHasValue invalidHV1 = new SMGEdgeHasValue(mockType, OFFSET0, object2b, value);
//
//    // Read 4 bytes (sizeof(mockType)) on offset 8 of 4b object -> out of bounds
//    SMGEdgeHasValue invalidHV2 = new SMGEdgeHasValue(mockType, OFFSET8, object4b, value);
//
//    smg1.addHasValueEdge(invalidHV1);
//    smg2.addHasValueEdge(invalidHV2);
//
//    Assert.assertFalse(SMGConsistencyVerifier.verifySMG(smg1));
//    Assert.assertFalse(SMGConsistencyVerifier.verifySMG(smg2));
//  }
//
//  @Test
//  public final void consistencyViolationHVConsistency() {
//    SMGObject object8b = new SMGRegion(SIZE8, "object_8b");
//    SMGObject object16b = new SMGRegion(SIZE10, "object_10b");
//
//    Integer firstValue = SMGValueFactory.getNewValue();
//    Integer secondValue = SMGValueFactory.getNewValue();
//
//    // 1, 3, 4 are consistent (different offsets or object)
//    // 2 is inconsistent with 1 (same object and offset, different value)
//    SMGEdgeHasValue hvEdge1 = new SMGEdgeHasValue(mockType, OFFSET0, object8b, firstValue);
//    SMGEdgeHasValue hvEdge2 = new SMGEdgeHasValue(mockType, OFFSET0, object8b, secondValue);
//    SMGEdgeHasValue hvEdge3 = new SMGEdgeHasValue(mockType, OFFSET4, object8b, secondValue);
//    SMGEdgeHasValue hvEdge4 = new SMGEdgeHasValue(mockType, OFFSET0, object16b, secondValue);
//
//    Assert.assertTrue(SMGConsistencyVerifier.verifySMG(emptySmg));
//
//    emptySmg.addHasValueEdge(hvEdge1);
//    Assert.assertFalse(SMGConsistencyVerifier.verifySMG(emptySmg));
//    emptySmg.addObject(object8b);
//    Assert.assertFalse(SMGConsistencyVerifier.verifySMG(emptySmg));
//    emptySmg.addValue(firstValue);
//    Assert.assertTrue(SMGConsistencyVerifier.verifySMG(emptySmg));
//
//    emptySmg.addHasValueEdge(hvEdge3);
//    Assert.assertFalse(SMGConsistencyVerifier.verifySMG(emptySmg));
//    emptySmg.addValue(secondValue);
//    Assert.assertTrue(SMGConsistencyVerifier.verifySMG(emptySmg));
//
//    emptySmg.addHasValueEdge(hvEdge4);
//    Assert.assertFalse(SMGConsistencyVerifier.verifySMG(emptySmg));
//    emptySmg.addObject(object16b);
//    Assert.assertTrue(SMGConsistencyVerifier.verifySMG(emptySmg));
//
//    emptySmg.addHasValueEdge(hvEdge2);
//    Assert.assertFalse(SMGConsistencyVerifier.verifySMG(emptySmg));
//  }
//
//  @Test
//  public final void consistencyViolationPTConsistency() {
//    SMGObject object8b = new SMGRegion(SIZE8, "object_8b");
//    SMGObject object16b = new SMGRegion(SIZE10, "object_10b");
//
//    Integer firstValue = SMGValueFactory.getNewValue();
//    Integer secondValue = SMGValueFactory.getNewValue();
//    Integer thirdValue = SMGValueFactory.getNewValue();
//
//    SMGEdgePointsTo edge1 = new SMGEdgePointsTo(firstValue, object8b, OFFSET0);
//    SMGEdgePointsTo edge2 = new SMGEdgePointsTo(thirdValue, object8b, OFFSET4);
//    SMGEdgePointsTo edge3 = new SMGEdgePointsTo(secondValue, object16b, OFFSET0);
//    SMGEdgePointsTo edge4 = new SMGEdgePointsTo(firstValue, object16b, OFFSET0);
//
//    Assert.assertTrue(SMGConsistencyVerifier.verifySMG(emptySmg));
//
//    emptySmg.addPointsToEdge(edge1);
//    Assert.assertFalse(SMGConsistencyVerifier.verifySMG(emptySmg));
//
//    emptySmg.addValue(firstValue);
//    Assert.assertFalse(SMGConsistencyVerifier.verifySMG(emptySmg));
//
//    emptySmg.addObject(object8b);
//    Assert.assertTrue(SMGConsistencyVerifier.verifySMG(emptySmg));
//
//    emptySmg.addPointsToEdge(edge2);
//    Assert.assertFalse(SMGConsistencyVerifier.verifySMG(emptySmg));
//
//    emptySmg.addValue(thirdValue);
//    Assert.assertTrue(SMGConsistencyVerifier.verifySMG(emptySmg));
//
//    emptySmg.addPointsToEdge(edge3);
//    Assert.assertFalse(SMGConsistencyVerifier.verifySMG(emptySmg));
//
//    emptySmg.addObject(object16b);
//    Assert.assertFalse(SMGConsistencyVerifier.verifySMG(emptySmg));
//
//    emptySmg.addValue(secondValue);
//    Assert.assertTrue(SMGConsistencyVerifier.verifySMG(emptySmg));
//
//    emptySmg.addPointsToEdge(edge4);
//    Assert.assertFalse(SMGConsistencyVerifier.verifySMG(emptySmg));
//  }
//
//  @Test(expected = IllegalArgumentException.class)
//  public final void isObjectValidBadCallTest() {
//    smg.isObjectValid(new SMGRegion(SIZE24, "wee"));
//  }
//
//  @Test(expected = IllegalArgumentException.class)
//  public final void setValidityBadCallTest() {
//    smg.setValidity(new SMGRegion(SIZE24, "wee"), true);
//  }
//
//  @Test
//  public final void getObjectsTest() {
//    HashSet<SMGObject> set = new HashSet<>();
//    set.add(obj1);
//    set.add(obj2);
//    set.add(smg.getNullObject());
//
//    Assert.assertTrue(smg.getObjects().containsAll(set));
//  }
//
//  @Test
//  public final void getNullObjectTest() {
//    SMGObject nullObject = smg.getNullObject();
//    Assert.assertFalse(smg.isObjectValid(nullObject));
//    Assert.assertEquals(nullObject.getSize(), 0);
//  }
//
//  @Test
//  public final void getValuesTest() {
//    HashSet<Integer> set = new HashSet<>();
//    set.add(val1);
//    set.add(val2);
//    set.add(smg.getNullValue());
//
//    Assert.assertTrue(smg.getValues().containsAll(set));
//  }
//
//  @Test
//  public final void getHVEdgesTest() {
//    Assert.assertTrue(Iterables.contains(smg.getHVEdges(), hv2has2at0));
//    Assert.assertTrue(Iterables.contains(smg.getHVEdges(), hv2has1at4));
//    Assert.assertEquals(2, Iterables.size(smg.getHVEdges()));
//  }
//
//  @Test
//  public final void getHVEdgesFilteredTest() {
//    Assert.assertEquals(0, Iterables.size(smg.getHVEdges(SMGEdgeHasValueFilter.objectFilter(obj1))));
//    Assert.assertEquals(2, Iterables.size(smg.getHVEdges(SMGEdgeHasValueFilter.objectFilter(obj2))));
//    Assert.assertTrue(Iterables.contains(smg.getHVEdges(SMGEdgeHasValueFilter.objectFilter(obj2)), hv2has2at0));
//    Assert.assertTrue(Iterables.contains(smg.getHVEdges(SMGEdgeHasValueFilter.objectFilter(obj2)), hv2has1at4));
//
//    Assert.assertEquals(1, Iterables.size(smg.getHVEdges(SMGEdgeHasValueFilter.objectFilter(obj2).filterAtOffset(0))));
//    Assert.assertTrue(Iterables.contains(smg.getHVEdges(SMGEdgeHasValueFilter.objectFilter(obj2).filterAtOffset(0)),
//                                         hv2has2at0));
//  }
//
//  @Test
//  public final void getPTEdgesTest() {
//    HashSet<SMGEdgePointsTo> set = new HashSet<>();
//    set.add(pt1to1);
//
//    Assert.assertTrue(smg.getPTEdges().containsAll(set));
//  }
//
//  @Test
//  public final void getObjectPointedByTest() {
//    Assert.assertEquals(obj1, smg.getObjectPointedBy(val1));
//    Assert.assertNull(smg.getObjectPointedBy(val2));
//  }
//
//  @Test
//  public final void neqBasicTest() {
//    NeqRelation nr = new NeqRelation();
//    Integer one = SMGValueFactory.getNewValue();
//    Integer two = SMGValueFactory.getNewValue();
//    Integer three = SMGValueFactory.getNewValue();
//
//    Assert.assertFalse(nr.neqExists(one, two));
//    Assert.assertFalse(nr.neqExists(one, three));
//    Assert.assertFalse(nr.neqExists(two, three));
//    Assert.assertFalse(nr.neqExists(two, one));
//    Assert.assertFalse(nr.neqExists(three, one));
//    Assert.assertFalse(nr.neqExists(three, two));
//
//    nr.addRelation(one, three);
//
//    Assert.assertFalse(nr.neqExists(one, two));
//    Assert.assertTrue(nr.neqExists(one, three));
//    Assert.assertFalse(nr.neqExists(two, three));
//    Assert.assertFalse(nr.neqExists(two, one));
//    Assert.assertTrue(nr.neqExists(three, one));
//    Assert.assertFalse(nr.neqExists(three, two));
//
//    nr.addRelation(one, three);
//
//    Assert.assertFalse(nr.neqExists(one, two));
//    Assert.assertTrue(nr.neqExists(one, three));
//    Assert.assertFalse(nr.neqExists(two, three));
//    Assert.assertFalse(nr.neqExists(two, one));
//    Assert.assertTrue(nr.neqExists(three, one));
//    Assert.assertFalse(nr.neqExists(three, two));
//
//    nr.addRelation(two, three);
//
//    Assert.assertFalse(nr.neqExists(one, two));
//    Assert.assertTrue(nr.neqExists(one, three));
//    Assert.assertTrue(nr.neqExists(two, three));
//    Assert.assertFalse(nr.neqExists(two, one));
//    Assert.assertTrue(nr.neqExists(three, one));
//    Assert.assertTrue(nr.neqExists(three, two));
//
//    nr.removeRelation(one, three);
//
//    Assert.assertFalse(nr.neqExists(one, two));
//    Assert.assertFalse(nr.neqExists(one, three));
//    Assert.assertTrue(nr.neqExists(two, three));
//    Assert.assertFalse(nr.neqExists(two, one));
//    Assert.assertFalse(nr.neqExists(three, one));
//    Assert.assertTrue(nr.neqExists(three, two));
//  }
//
//  @Test
//  public final void neqPutAllTest() {
//    NeqRelation nr = new NeqRelation();
//    NeqRelation newNr = new NeqRelation();
//    Integer one = SMGValueFactory.getNewValue();
//    Integer two = SMGValueFactory.getNewValue();
//    Integer three = SMGValueFactory.getNewValue();
//
//    nr.addRelation(one, three);
//
//    newNr.putAll(nr);
//    Assert.assertFalse(nr.neqExists(one, two));
//    Assert.assertTrue(nr.neqExists(one, three));
//    Assert.assertFalse(nr.neqExists(two, three));
//    Assert.assertFalse(newNr.neqExists(two, one));
//    Assert.assertTrue(newNr.neqExists(three, one));
//    Assert.assertFalse(newNr.neqExists(three, two));
//
//    nr.removeRelation(one, three);
//    Assert.assertFalse(nr.neqExists(one, two));
//    Assert.assertFalse(nr.neqExists(one, three));
//    Assert.assertFalse(nr.neqExists(two, three));
//    Assert.assertFalse(newNr.neqExists(two, one));
//    Assert.assertTrue(newNr.neqExists(three, one));
//    Assert.assertFalse(newNr.neqExists(three, two));
//  }
//
//  @Test
//  public final void neqRemoveValueTest() {
//    NeqRelation nr = new NeqRelation();
//    Integer one = SMGValueFactory.getNewValue();
//    Integer two = SMGValueFactory.getNewValue();
//    Integer three = SMGValueFactory.getNewValue();
//
//    nr.addRelation(one, two);
//    nr.addRelation(one, three);
//    nr.removeValue(one);
//    Assert.assertFalse(nr.neqExists(one, two));
//    Assert.assertFalse(nr.neqExists(one, three));
//    Assert.assertFalse(nr.neqExists(two, three));
//  }
//
//  @Test
//  public final void neqMergeValuesTest() {
//    NeqRelation nr = new NeqRelation();
//    Integer one = SMGValueFactory.getNewValue();
//    Integer two = SMGValueFactory.getNewValue();
//    Integer three = SMGValueFactory.getNewValue();
//
//    nr.addRelation(one, three);
//    nr.mergeValues(two, three);
//
//    Assert.assertTrue(nr.neqExists(one, two));
//    Assert.assertFalse(nr.neqExists(one, three));
//    Assert.assertFalse(nr.neqExists(two, three));
//  }
//
//  @Test
//  public final void neqMergeValuesTest2() {
//    NeqRelation nr = new NeqRelation();
//    Integer zero = SMGValueFactory.getNewValue();
//    Integer one = SMGValueFactory.getNewValue();
//    Integer two = SMGValueFactory.getNewValue();
//    Integer three = SMGValueFactory.getNewValue();
//
//    nr.addRelation(zero, three);
//    nr.addRelation(one, three);
//    nr.mergeValues(two, three);
//
//    Assert.assertTrue(nr.neqExists(zero, two));
//    Assert.assertTrue(nr.neqExists(one, two));
//    Assert.assertFalse(nr.neqExists(zero, three));
//    Assert.assertFalse(nr.neqExists(one, three));
//    Assert.assertFalse(nr.neqExists(two, three));
//  }
//}
