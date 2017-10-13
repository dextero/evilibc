#include "Test.h"

extern "C" int test_strncmp(const char *, const char *, size_t);

class StrncmpTest : public evil::Test {};

TEST_F(StrncmpTest, null) {
    {
        evil::UBChecker checker{1};
        ASSERT_NE(0, test_strncmp(NULL, "", 0));
    }
    {
        evil::UBChecker checker{1};
        ASSERT_NE(0, test_strncmp("", NULL, 0));
    }
    {
        evil::UBChecker checker{1};
        ASSERT_NE(0, test_strncmp(NULL, NULL, 0));
    }
}

TEST_F(StrncmpTest, equal) {
    ASSERT_EQ(0, test_strncmp("", "", 0));
    ASSERT_EQ(0, test_strncmp("", "", sizeof("")));
    ASSERT_EQ(0, test_strncmp("abc", "abc", sizeof("abc")));
}

TEST_F(StrncmpTest, first_less) {
    ASSERT_LT(0, test_strncmp("a", "b", sizeof("a")));
    ASSERT_LT(0, test_strncmp("aa", "ab", sizeof("ab")));
    ASSERT_LT(0, test_strncmp("ab", "ba", sizeof("ab")));
}

TEST_F(StrncmpTest, first_greater) {
    ASSERT_GT(0, test_strncmp("b", "a", sizeof("a")));
    ASSERT_GT(0, test_strncmp("ab", "aa", sizeof("ab")));
    ASSERT_GT(0, test_strncmp("ba", "ab", sizeof("ab")));
}

TEST_F(StrncmpTest, n_smaller_than_length) {
    ASSERT_EQ(0, test_strncmp("ab", "ac", 1));
}

TEST_F(StrncmpTest, difference_after_nullbyte) {
    ASSERT_EQ(0, test_strncmp("a\0b", "a\0c", sizeof("a\0b")));
}
