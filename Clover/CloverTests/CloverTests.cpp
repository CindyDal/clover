#include "gTest/gtest.h"
#include "../Clover/server.h"

TEST(SquareTests, Square)
{
	EXPECT_EQ(0, square(0));
	EXPECT_EQ(-4, square(-2));
}