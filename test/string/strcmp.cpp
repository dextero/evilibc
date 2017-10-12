#include "Test.h"

extern "C" int test_strcmp(const char *, const char *);

class StrcmpTest : public evil::Test {};

TEST_F(StrcmpTest, null) {
    {
        evil::UBChecker checker{1};
        ASSERT_NE(0, test_strcmp(NULL, ""));
    }
    {
        evil::UBChecker checker{1};
        ASSERT_NE(0, test_strcmp("", NULL));
    }
    {
        evil::UBChecker checker{1};
        ASSERT_NE(0, test_strcmp(NULL, NULL));
    }
}

TEST_F(StrcmpTest, equal) {
    ASSERT_EQ(0, test_strcmp("", ""));
    ASSERT_EQ(0, test_strcmp("abc", "abc"));
}

TEST_F(StrcmpTest, first_less) {
    ASSERT_LT(0, test_strcmp("a", "b"));
    ASSERT_LT(0, test_strcmp("aa", "ab"));
    ASSERT_LT(0, test_strcmp("ab", "ba"));
}

TEST_F(StrcmpTest, first_greater) {
    ASSERT_GT(0, test_strcmp("b", "a"));
    ASSERT_GT(0, test_strcmp("ab", "aa"));
    ASSERT_GT(0, test_strcmp("ba", "ab"));
}
