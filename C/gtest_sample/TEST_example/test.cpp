#include "add.h"
#include <gtest/gtest.h>

TEST(AddTest, Handletest1) {
	EXPECT_EQ(1, add(0, 1));
}

TEST(AddTest, Handletest2) {
	EXPECT_EQ(2, add(1, 1));
	EXPECT_EQ(3, add(1, 2));
}

/*
int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
} */
