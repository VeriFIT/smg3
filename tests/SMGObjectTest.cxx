/*
 * SMGObjectTest.cxx
 *
 *  Created on: Dec 3, 2015
 *      Author: pmuller
 */

/*
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
// #include <SMGObject.h>

int SIZE8 = 8;
int SIZE12 = 12;

class SMGObject {
	static SMGObject SMGObject::getNullObject() {
		return nullptr;
	}
	bool notNull() {
		return false;
	}
	bool isAbstract() {
		return false;
	}
	bool isMoreGeneral(SMGObject* pOther) {
		return false;
	}
	SMGObject join(SMGObject pOther) {
		return nullptr;
	}
	int getSize() {
		return 0;
	}
	const std::string& getLabel() {
		return nullptr;
	}
};

class TestingObject : private SMGObject {
	private:
		int size;
		std::string label;
	public:
	TestingObject(int pSize, std::string pLabel) {
		this->size = pSize;
		this->label = pLabel;
	}
	TestingObject(const TestingObject&) = default;

	const std::string& getLabel() {
		return this->label;
	}
};

TestingObject *object8 = new TestingObject(8, "label");
TestingObject *object12 = new TestingObject(12, "another label");

TEST(SMGObject, getNullObjectTest) {
	SMGObject *nullObject = SMGObject::getNullObject();
	EXPECT_FALSE(nullObject->notNull());
	EXPECT_TRUE(object8->notNull());
	EXPECT_FALSE(nullObject->isAbstract());
	EXPECT_FALSE(nullObject->isMoreGeneral(object8));
	EXPECT_FALSE(nullObject->isMoreGeneral(nullObject));
	EXPECT_EQ(nullObject, nullObject->join(*nullObject));
	EXPECT_EQ(*object8, nullObject->join(*object8));
}

//	public final void testSMGObjectIntString() {
//    Assert.assertEquals(SIZE8, object8.getSize());
//    Assert.assertEquals("label", object8.getLabel());
//    Assert.assertEquals(SIZE12, object12.getSize());
//    Assert.assertEquals("another label", object12.getLabel());
//    SMGObject object12copy = new TestingObject(object12);
//    Assert.assertEquals(SIZE12, object12copy.getSize());
//    Assert.assertEquals("another label", object12copy.getLabel());
//  }

TEST(SMGObject, PropertySanity){
	EXPECT_EQ(SIZE8, object8->getSize());
	EXPECT_STREQ("label", object8->getLabel().c_str());
	EXPECT_EQ(SIZE12, object12->getSize());
	EXPECT_STREQ("another label", object12->getLabel().c_str());
	SMGObject* object12Copy = new TestingObject(object12);
	EXPECT_EQ(SIZE12, object12Copy->getSize());
	EXPECT_STREQ("another label", object12Copy->getLabel().c_str());
}


//  private static class TestingObject extends SMGObject {
//
//    protected TestingObject(final int pSize, final String pLabel) {
//      super(pSize, pLabel);
//    }
//
//    protected TestingObject(final TestingObject pOther) {
//      super(pOther);
//    }
//
//    @Override
//    public boolean isAbstract() {
//      return false;
//    }
//
//    @Override
//    public void accept(final SMGObjectVisitor pVisitor) {
//      pVisitor.visit(this);
//    }
//
//    @Override
//    public boolean isMoreGeneral(final SMGObject pOther) {
//      return false;
//    }
//
//    @Override
//    public SMGObject join(final SMGObject pOther) {
//      return pOther;
//    }
//  }
//
//  @Test
//	public final void testVisitorOnNull() {
//    SMGObject nullObject = SMGObject.getNullObject();
//    SMGObjectVisitor visitor = new SMGObjectVisitor() {
//      @Override
//      public void visit(final SMGObject pObject) {
//        Assert.assertFalse(pObject.notNull());
//      }
//    };
//    nullObject.accept(visitor);
//  }
//}
