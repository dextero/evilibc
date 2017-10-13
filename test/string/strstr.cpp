#include "Test.h"

#include <string>

using namespace std;

extern "C" char *test_strstr(const char *, const char *);

class StrstrTest : public evil::Test {};

TEST_F(StrstrTest, null_str) {
    evil::UBChecker checker{1};
    ASSERT_EQ(NULL, test_strstr(NULL, ""));
}

TEST_F(StrstrTest, empty_str) {
    string s = "a";
    ASSERT_EQ(&s[0], test_strstr(&s[0], ""));
}

TEST_F(StrstrTest, no_occurrences) {
    string s = "a";
    ASSERT_EQ(NULL, test_strstr(&s[0], "b"));
    ASSERT_EQ(NULL, test_strstr(&s[0], "ab"));
}

TEST_F(StrstrTest, single_occurrence) {
    string s = "abc";
    ASSERT_EQ(&s[1], test_strstr(&s[0], "b"));
    ASSERT_EQ(&s[1], test_strstr(&s[0], "bc"));
    ASSERT_EQ(&s[0], test_strstr(&s[0], "ab"));
}

TEST_F(StrstrTest, multiple_occurrence) {
    string s = "abab";
    ASSERT_EQ(&s[0], test_strstr(&s[0], "a"));
    ASSERT_EQ(&s[0], test_strstr(&s[0], "ab"));
}
