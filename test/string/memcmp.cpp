#include "Test.h"

extern "C" int test_memcmp(const void *, const void *, size_t);

class MemcmpTest : public evil::Test {};

TEST_F(MemcmpTest, null) {
    {
        evil::UBChecker checker{1};
        ASSERT_NE(0, test_memcmp(NULL, "", 0));
    }
    {
        evil::UBChecker checker{1};
        ASSERT_NE(0, test_memcmp("", NULL, 0));
    }
    {
        evil::UBChecker checker{1};
        ASSERT_NE(0, test_memcmp(NULL, NULL, 0));
    }
}

TEST_F(MemcmpTest, equal) {
    ASSERT_EQ(0, test_memcmp("", "", 0));
    ASSERT_EQ(0, test_memcmp("", "", sizeof("")));
    ASSERT_EQ(0, test_memcmp("abc", "abc", sizeof("abc")));
}

TEST_F(MemcmpTest, first_less) {
    ASSERT_LT(0, test_memcmp("a", "b", sizeof("a")));
    ASSERT_LT(0, test_memcmp("aa", "ab", sizeof("ab")));
    ASSERT_LT(0, test_memcmp("ab", "ba", sizeof("ab")));
}

TEST_F(MemcmpTest, first_greater) {
    ASSERT_GT(0, test_memcmp("b", "a", sizeof("a")));
    ASSERT_GT(0, test_memcmp("ab", "aa", sizeof("ab")));
    ASSERT_GT(0, test_memcmp("ba", "ab", sizeof("ab")));
}
