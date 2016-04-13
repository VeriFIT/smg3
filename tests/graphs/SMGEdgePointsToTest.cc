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
#include "graphs/SMGEdgePointsTo.hh"
#include "graphs/SMGValue.hh"
#include "objects/SMGObject.hh"
#include "objects/SMGRegion.hh"

namespace smg {

const int SIZE8 = 8;
const int OFFSET0 = 0;
const int OFFSET4 = 4;

TEST(SMGEdgePointsTo, Sanity) {
  const SMGValue value = SMGValue::GetNewValue();
  const SMGObjectPtr object = std::make_shared<SMGRegion>(SIZE8, "object");
  const SMGEdgePointsTo edge = SMGEdgePointsTo(value, object, 0);

  EXPECT_EQ(value.GetId(), edge.GetValue().GetId());
  EXPECT_EQ(object->GetId(), edge.GetObject()->GetId());
  EXPECT_EQ(0, edge.GetOffset());
}

TEST(SMGEdgePointsTo, IsConsistentWith) {
  const SMGValue val_1 = SMGValue::GetNewValue();
  const SMGValue val_2 = SMGValue::GetNewValue();
  const SMGObjectPtr obj_1 = std::make_shared<SMGRegion>(SIZE8, "object 1");
  const SMGObjectPtr obj_2 = std::make_shared<SMGRegion>(SIZE8, "object 2");

  const SMGEdgePointsTo& edge_1 = SMGEdgePointsTo(val_1, obj_1, OFFSET0);
  const SMGEdgePointsTo& edge_2 = SMGEdgePointsTo(val_2, obj_1, OFFSET0);
  const SMGEdgePointsTo& edge_3 = SMGEdgePointsTo(val_1, obj_1, OFFSET4);
  const SMGEdgePointsTo& edge_4 = SMGEdgePointsTo(val_1, obj_2, OFFSET0);

  // An edge is consistent with itself
  EXPECT_TRUE(edge_1.IsConsistentWith(edge_1));

  // Different vals pointing to same place: violates "injective"
  EXPECT_FALSE(edge_1.IsConsistentWith(edge_2));

  // Same val pointing to different offsets
  EXPECT_FALSE(edge_1.IsConsistentWith(edge_3));

  // Same val pointing to different objects
  EXPECT_FALSE(edge_1.IsConsistentWith(edge_4));
}

}  // namespace smg
