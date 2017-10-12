#include "Test.h"

#include <string>

using namespace std;
using namespace std::literals;

extern "C" char *test_strcat(char *, const char *);

class StrcatTest : public evil::Test {};

TEST_F(StrcatTest, null_str) {
    {
        evil::UBChecker checker{1};
        ASSERT_EQ(NULL, test_strcat(NULL, ""));
    }
    {
        evil::UBChecker checker{1};
        char buf[1];
        ASSERT_EQ(buf, test_strcat(buf, NULL));
    }
}

TEST_F(StrcatTest, overlapping) {
    string s = "ab";
    {
        evil::UBChecker checker{1};
        ASSERT_EQ(&s[0], test_strcat(&s[0], &s[0]));
        ASSERT_EQ("ab"s, s);
    }
    {
        evil::UBChecker checker{1};
        ASSERT_EQ(&s[0], test_strcat(&s[0], &s[1]));
        ASSERT_EQ("ab"s, s);
    }
    {
        evil::UBChecker checker{1};
        ASSERT_EQ(&s[1], test_strcat(&s[1], &s[0]));
        ASSERT_EQ("ab"s, s);
    }
}

TEST_F(StrcatTest, non_overlapping) {
    string s1 = "a\0"s;
    string s2 = "b";

    ASSERT_EQ(&s1[0], test_strcat(&s1[0], &s2[0]));
    ASSERT_EQ("ab"s, s1);
}
