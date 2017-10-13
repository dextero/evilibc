#include "Test.h"

#include <string>

using namespace std;

extern "C" char *test_strpbrk(const char *, const char *);

class StrpbrkTest : public evil::Test {};

TEST_F(StrpbrkTest, null) {
    {
        evil::UBChecker checker{1};
        ASSERT_EQ(NULL, test_strpbrk(NULL, ""));
    }
    {
        evil::UBChecker checker{1};
        ASSERT_EQ(NULL, test_strpbrk("", NULL));
    }
    {
        evil::UBChecker checker{1};
        ASSERT_EQ(NULL, test_strpbrk(NULL, NULL));
    }
}

TEST_F(StrpbrkTest, empty) {
    ASSERT_EQ(NULL, test_strpbrk("abc", ""));
}

TEST_F(StrpbrkTest, single_char) {
    string s = "abc";
    ASSERT_EQ(&s[0], test_strpbrk(&s[0], "a"));
    ASSERT_EQ(&s[1], test_strpbrk(&s[0], "b"));
}

TEST_F(StrpbrkTest, multiple_chars) {
    string s = "abc";
    ASSERT_EQ(&s[0], test_strpbrk(&s[0], "ba"));
    ASSERT_EQ(&s[0], test_strpbrk(&s[0], "ab"));
}

TEST_F(StrpbrkTest, returns_first_result) {
    string s = "aaa";
    ASSERT_EQ(&s[0], test_strpbrk(&s[0], "a"));
}
