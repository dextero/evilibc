#include "Test.h"

#include <string>

using namespace std;

extern "C" size_t test_strcspn(const char *, const char *);

class StrcspnTest : public evil::Test {};

TEST_F(StrcspnTest, null_s1) {
    evil::UBChecker checker{1};
    ASSERT_EQ(0u, test_strcspn(NULL, ""));
}

TEST_F(StrcspnTest, null_s2) {
    evil::UBChecker checker{1};
    ASSERT_EQ(0u, test_strcspn("", NULL));
}

TEST_F(StrcspnTest, empty_s1) {
    ASSERT_EQ(0u, test_strcspn("", "a"));
}

TEST_F(StrcspnTest, empty_s2) {
    ASSERT_EQ(1u, test_strcspn("a", ""));
}

TEST_F(StrcspnTest, single_char_in_s2) {
    ASSERT_EQ(1u, test_strcspn("ba", "a"));
    ASSERT_EQ(2u, test_strcspn("bcaba", "a"));
}

TEST_F(StrcspnTest, multiple_chars_in_s2) {
    ASSERT_EQ(2u, test_strcspn("cdba", "ab"));
}
