/*
 * SMGPointsToEdgeTest.cc
 *
 *  This file is part of SMG, a symbolic memory graph Java library
 *  Originally developed as part of CPAChecker, the configurable software verification platform
 *
 *  Copyright (C) 2011-2015  Petr Muller
 *  Copyright (C) 2007-2014  Dirk Beyer
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
#include <SMGValue.hh>
#include <SMGObject.hh>
#include <SMGRegion.hh>

const int SIZE8 = 8;
const int OFFSET0 = 0;
const int OFFSET4 = 4;

TEST(SMGEdgePointsTo, sanity) {
    SMGValue value = SMGValue::getNewValue();
    const SMGObject &object = SMGRegion(SIZE8, "object");
    SMGEdgePointsTo edge = SMGEdgePointsTo(value, object, 0);

    EXPECT_EQ(value.getId(), edge.getValue().getId());
    EXPECT_EQ(object.getId(), edge.getObject().getId());
    EXPECT_EQ(0, edge.getOffset());
}

TEST(SMGEdgePointsTo, isConsistentWith) {
    const SMGValue &val1 = SMGValue::getNewValue();
    const SMGValue &val2 = SMGValue::getNewValue();
    const SMGObject& obj1 = SMGRegion(SIZE8, "object 1");
    const SMGObject& obj2 = SMGRegion(SIZE8, "object 2");

    const SMGEdgePointsTo& edge1 = SMGEdgePointsTo(val1, obj1, OFFSET0);
    const SMGEdgePointsTo& edge2 = SMGEdgePointsTo(val2, obj1, OFFSET0);
    const SMGEdgePointsTo& edge3 = SMGEdgePointsTo(val1, obj1, OFFSET4);
    const SMGEdgePointsTo& edge4 = SMGEdgePointsTo(val1, obj2, OFFSET0);

    // An edge is consistent with itself
    EXPECT_TRUE(edge1.isConsistentWith(edge1));

    // Different vals pointing to same place: violates "injective"
    EXPECT_FALSE(edge1.isConsistentWith(edge2));

    // Same val pointing to different offsets
    EXPECT_FALSE(edge1.isConsistentWith(edge3));

    // Same val pointing to different objects
    EXPECT_FALSE(edge1.isConsistentWith(edge4));
}
