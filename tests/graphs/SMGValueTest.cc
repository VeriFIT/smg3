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
#include "graphs/SMGValue.hh"

namespace smg {

TEST(SMGValue, GetNewValue) {
  SMGValue value_1 = SMGValue::GetNewValue();
  SMGValue value_2 = SMGValue::GetNewValue();
  SMGValue value_3 = SMGValue::GetNewValue();
  EXPECT_NE(value_1.GetId(), value_2.GetId());
  EXPECT_NE(value_1.GetId(), value_3.GetId());
  EXPECT_NE(value_2.GetId(), value_3.GetId());
}

}  // namespace smg
