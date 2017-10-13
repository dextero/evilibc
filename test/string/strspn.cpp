#include "Test.h"

#include <string>

using namespace std;

extern "C" size_t test_strspn(const char *, const char *);

class StrspnTest : public evil::Test {};

TEST_F(StrspnTest, null_s1) {
    evil::UBChecker checker{1};
    ASSERT_EQ(0u, test_strspn(NULL, ""));
}

TEST_F(StrspnTest, null_s2) {
    evil::UBChecker checker{1};
    ASSERT_EQ(0u, test_strspn("", NULL));
}

TEST_F(StrspnTest, empty_s1) {
    ASSERT_EQ(0u, test_strspn("", "a"));
}

TEST_F(StrspnTest, empty_s2) {
    ASSERT_EQ(0u, test_strspn("a", ""));
}

TEST_F(StrspnTest, single_char_in_s2) {
    ASSERT_EQ(2u, test_strspn("aaba", "a"));
    ASSERT_EQ(0u, test_strspn("aaba", "c"));
}

TEST_F(StrspnTest, multiple_chars_in_s2) {
    ASSERT_EQ(5u, test_strspn("aababc", "ab"));
}
