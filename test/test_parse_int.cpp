

#include <gtest/gtest.h>
#include "parse_int.h"

TEST(ParseInt, NoNumber)
{
    EXPECT_EQ(parse_int_branchless(""), 0);
    EXPECT_EQ(parse_int_branchless("abc"), 0);
    EXPECT_EQ(parse_int_branchless("-abc"), 0);

}

TEST(ParseInt, UnsignNumber)
{
    EXPECT_EQ(parse_int_branchless("5"), 5);
    EXPECT_EQ(parse_int_branchless("123"), 123);
    EXPECT_EQ(parse_int_branchless("46789"), 46789);
    EXPECT_EQ(parse_int_branchless("0"), 0);

}

TEST(ParseInt, SignNumber)
{
    EXPECT_EQ(parse_int_branchless("-5"), -5);
    EXPECT_EQ(parse_int_branchless("-123"), -123);
    EXPECT_EQ(parse_int_branchless("-46789"), -46789);
    EXPECT_EQ(parse_int_branchless("-0"), -0);

}