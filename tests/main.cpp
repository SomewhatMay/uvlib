#include <iostream>

#include "gtest/gtest.h"

TEST(MainTest, Multiply) { ASSERT_EQ(true, true) << "Hello world!"; }

TEST(MainTest, test) { std::cout << "CPP Version: " << __cplusplus << "\n"; }