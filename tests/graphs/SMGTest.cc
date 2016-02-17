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
#include "graphs/SMG.hh"
#include "graphs/SMGConsistencyVerifier.hh"
#include "graphs/SMGEdgeHasValue.hh"
#include "graphs/SMGEdgePointsTo.hh"
#include "graphs/SMGValue.hh"
#include "objects/SMGObject.hh"
#include "objects/SMGRegion.hh"
#include "types/SMGCType.hh"

namespace smg {

const int SIZE4 = 4;
const int SIZE8 = 8;
const int SIZE16 = 16;

const int OFFSET0 = 0;
const int OFFSET4 = 4;

const SMGCType& mock_type = SMGCType::CreateTypeWithSize(SIZE4);

// obj1 = xxxxxxxx
// obj2 = yyyyzzzz
// val1 -> obj1
// yyyy has value 2
// zzzz has value 1

const SMGObjectPtr obj_1 = std::make_shared<SMGRegion>(SIZE8, "object-1");
const SMGObjectPtr obj_2 = std::make_shared<SMGRegion>(SIZE8, "object-2");

const SMGValue val_1 = SMGValue::GetNewValue();
const SMGValue val_2 = SMGValue::GetNewValue();

const SMGEdgePointsToPtr pt_1_to_1 = std::make_shared<SMGEdgePointsTo>(val_1, obj_1, OFFSET0);
const SMGEdgeHasValuePtr hv_2_has_2_at_0 =
    std::make_shared<SMGEdgeHasValue>(mock_type, OFFSET0, obj_2, val_2);
const SMGEdgeHasValuePtr hv_2_has_1_at_4 =
    std::make_shared<SMGEdgeHasValue>(mock_type, OFFSET4, obj_2, val_1);

class SMGTest : public testing::Test {
 protected:
  SMG smg = SMG();
  SMG empty_smg = SMG();

  virtual void setUp() {
    smg.AddObject(obj_1);
    smg.AddObject(obj_2);

    smg.AddValue(val_1);
    smg.AddValue(val_2);

    smg.AddPointsToEdge(pt_1_to_1);

    smg.AddHasValueEdge(hv_2_has_2_at_0);
    smg.AddHasValueEdge(hv_2_has_1_at_4);
  }
};

TEST_F(SMGTest, Constructor) {
  EXPECT_TRUE(SMGConsistencyVerifier::Verify(empty_smg));
  const SMGObjectPtr null_object = empty_smg.GetNullObject();
  const SMGValue null_address = empty_smg.GetNullValue();

  EXPECT_FALSE(null_object->NotNull());
  EXPECT_EQ(1, empty_smg.GetObjects().size());
  EXPECT_TRUE(empty_smg.GetObjects().contains(null_object));

  EXPECT_EQ(1, empty_smg.GetValues().size());
  EXPECT_TRUE(empty_smg.GetValues().count(null_address));

  EXPECT_EQ(1, empty_smg.GetPTEdges().size());
  const SMGObjectPtr target_object = empty_smg.GetObjectPointedBy(null_address);
  EXPECT_EQ(null_object->GetId(), target_object->GetId());

  EXPECT_TRUE(empty_smg.GetHVEdges().empty());

  // COPY CONSTRUCTOR

  SMG smg_copy = SMG(empty_smg);

  EXPECT_TRUE(SMGConsistencyVerifier::Verify(empty_smg));
  EXPECT_TRUE(SMGConsistencyVerifier::Verify(smg_copy));

  const SMGObjectPtr third_object = std::make_shared<SMGRegion>(SIZE16, "object-3");
  const SMGValue third_value = SMGValue::GetNewValue();
  smg_copy.AddObject(third_object);
  smg_copy.AddValue(third_value);
  smg_copy.AddHasValueEdge(
      std::make_shared<SMGEdgeHasValue>(mock_type, OFFSET0, third_object, third_value));
  smg_copy.AddPointsToEdge(std::make_shared<SMGEdgePointsTo>(third_value, third_object, OFFSET0));

  EXPECT_TRUE(SMGConsistencyVerifier::Verify(empty_smg));
  EXPECT_TRUE(SMGConsistencyVerifier::Verify(smg_copy));
  EXPECT_EQ(1, empty_smg.GetObjects().size());
  EXPECT_EQ(2, smg_copy.GetObjects().size());
  EXPECT_EQ(1, smg_copy.GetObjects().contains(third_object));

  EXPECT_EQ(1, empty_smg.GetValues().size());
  EXPECT_EQ(2, smg_copy.GetValues().size());
  EXPECT_EQ(1, smg_copy.GetValues().count(third_value));

  EXPECT_EQ(1, empty_smg.GetPTEdges().size());
  EXPECT_EQ(2, smg_copy.GetPTEdges().size());
  const SMGObjectPtr target_object_for_third = smg_copy.GetObjectPointedBy(third_value);
  EXPECT_EQ(third_object.get(), target_object_for_third.get());

  EXPECT_TRUE(empty_smg.GetHVEdges().empty());
  EXPECT_EQ(1, smg_copy.GetHVEdges().size());
}

TEST_F(SMGTest, AddRemoveHasValueEdge) {
  const SMGObjectPtr object = std::make_shared<SMGRegion>(SIZE4, "object");

  const SMGEdgeHasValuePtr
      hv = std::make_shared<SMGEdgeHasValue>(mock_type, OFFSET0, object, SMGValue::GetNullValue());

  empty_smg.AddHasValueEdge(hv);
  EXPECT_TRUE(empty_smg.GetHVEdges().contains(hv));

  empty_smg.RemoveHasValueEdge(hv);
  EXPECT_FALSE(empty_smg.GetHVEdges().contains(hv));
}

TEST_F(SMGTest, RemoveObject) {
  const SMGValue new_value = SMGValue::GetNewValue();

  const SMGObjectPtr object = std::make_shared<SMGRegion>(SIZE8, "object");
  const SMGEdgeHasValuePtr hv_0 =
      std::make_shared<SMGEdgeHasValue>(mock_type, OFFSET0, object, SMGValue::GetNullValue());
  const SMGEdgeHasValuePtr hv_4 =
      std::make_shared<SMGEdgeHasValue>(mock_type, OFFSET4, object, SMGValue::GetNullValue());
  const SMGEdgePointsToPtr pt = std::make_shared<SMGEdgePointsTo>(new_value, object, OFFSET0);

  empty_smg.AddValue(new_value);
  empty_smg.AddObject(object);
  empty_smg.AddPointsToEdge(pt);
  empty_smg.AddHasValueEdge(hv_0);
  empty_smg.AddHasValueEdge(hv_4);

  EXPECT_TRUE(empty_smg.GetObjects().contains(object));
  empty_smg.RemoveObject(object);
  EXPECT_FALSE(empty_smg.GetObjects().contains(object));
  const auto pt_edges_map = empty_smg.GetPTEdges();
  SMGEntitySet<const SMGEdgePointsTo> pt_edges;
  for (auto pti : pt_edges_map) {
    pt_edges.add(pti.second);
  }
  EXPECT_TRUE(pt_edges.contains(pt));

  EXPECT_TRUE(empty_smg.GetHVEdges().contains(hv_0));
  EXPECT_TRUE(empty_smg.GetHVEdges().contains(hv_4));
}

TEST_F(SMGTest, RemoveObjectAndEdges) {
  const SMGValue new_value = SMGValue::GetNewValue();

  const SMGObjectPtr object = std::make_shared<SMGRegion>(SIZE8, "object");
  const SMGEdgeHasValuePtr hv_0 =
      std::make_shared<SMGEdgeHasValue>(mock_type, OFFSET0, object, SMGValue::GetNullValue());
  const SMGEdgeHasValuePtr hv_4 =
      std::make_shared<SMGEdgeHasValue>(mock_type, OFFSET4, object, SMGValue::GetNullValue());
  const SMGEdgePointsToPtr pt = std::make_shared<SMGEdgePointsTo>(new_value, object, OFFSET0);

  empty_smg.AddValue(new_value);
  empty_smg.AddObject(object);
  empty_smg.AddPointsToEdge(pt);
  empty_smg.AddHasValueEdge(hv_0);
  empty_smg.AddHasValueEdge(hv_4);

  EXPECT_TRUE(empty_smg.GetObjects().contains(object));
  empty_smg.RemoveObjectAndEdges(object);
  EXPECT_FALSE(empty_smg.GetObjects().contains(object));
  const auto pt_edges_map = empty_smg.GetPTEdges();
  SMGEntitySet<const SMGEdgePointsTo> pt_edges;
  for (auto pti : pt_edges_map) {
    pt_edges.add(pti.second);
  }
  EXPECT_FALSE(pt_edges.contains(pt));
  EXPECT_FALSE(empty_smg.GetHVEdges().contains(hv_0));
  EXPECT_FALSE(empty_smg.GetHVEdges().contains(hv_4));
}

TEST_F(SMGTest, GetNullObject) {
  const auto null_object = smg.GetNullObject();
  EXPECT_FALSE(smg.IsObjectValid(null_object));
  EXPECT_EQ(null_object->GetSize(), 0);
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
//    Assert.assertEquals(0,
// Iterables.size(smg.getHVEdges(SMGEdgeHasValueFilter.objectFilter(obj1))));
//    Assert.assertEquals(2,
// Iterables.size(smg.getHVEdges(SMGEdgeHasValueFilter.objectFilter(obj2))));
//    Assert.assertTrue(Iterables.contains(smg.getHVEdges(SMGEdgeHasValueFilter.objectFilter(obj2)),
// hv2has2at0));
//    Assert.assertTrue(Iterables.contains(smg.getHVEdges(SMGEdgeHasValueFilter.objectFilter(obj2)),
// hv2has1at4));
//
//    Assert.assertEquals(1,
// Iterables.size(smg.getHVEdges(SMGEdgeHasValueFilter.objectFilter(obj2).filterAtOffset(0))));
//    Assert.assertTrue(Iterables.contains(smg.getHVEdges(SMGEdgeHasValueFilter.objectFilter(obj2).
//                                         filterAtOffset(0)), hv2has2at0));
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

}  // namespace smg
