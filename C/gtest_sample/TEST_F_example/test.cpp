#include "foo.h"
#include <gtest/gtest.h>

namespace {

class FooTest : public ::testing::Test {
protected:
	FooTest() {
	}

	virtual ~FooTest() {
	}

	virtual void SetUp() {
		f = new Foo();
	}

	virtual void TearDown() {
		delete f;
	}

	Foo* f;
}; //FooTest

TEST_F(FooTest, Add) {
	EXPECT_EQ(2, f->add(1, 1));
}

} //namespace

/*
int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
*/
