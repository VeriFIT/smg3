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
#include <SMGValue.hh>

TEST(SMGValue, SMGValue_getNewValue) {
	SMGValue value1 = SMGValue::getNewValue();
	SMGValue value2 = SMGValue::getNewValue();
	SMGValue value3 = SMGValue::getNewValue();
	EXPECT_NE(value1.getId(), value2.getId());
	EXPECT_NE(value1.getId(), value3.getId());
	EXPECT_NE(value2.getId(), value3.getId());
}
