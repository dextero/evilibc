#include "Test.h"

#include <string>

using namespace std;
using namespace std::literals;

extern "C" char *test_strncat(char *, const char *, size_t);

class StrncatTest : public evil::Test {};

TEST_F(StrncatTest, null_str) {
    {
        evil::UBChecker checker{1};
        ASSERT_EQ(NULL, test_strncat(NULL, "", 0));
    }
    {
        evil::UBChecker checker{1};
        char buf[1];
        ASSERT_EQ(buf, test_strncat(buf, NULL, 0));
    }
}

TEST_F(StrncatTest, overlapping) {
    string s = "ab";
    {
        evil::UBChecker checker{1};
        ASSERT_EQ(&s[0], test_strncat(&s[0], &s[0], 0));
        ASSERT_EQ("ab"s, s);
    }
    {
        evil::UBChecker checker{1};
        ASSERT_EQ(&s[0], test_strncat(&s[0], &s[1], 0));
        ASSERT_EQ("ab"s, s);
    }
    {
        evil::UBChecker checker{1};
        ASSERT_EQ(&s[1], test_strncat(&s[1], &s[0], 0));
        ASSERT_EQ("ab"s, s);
    }
}

TEST_F(StrncatTest, non_overlapping) {
    string s1 = "a\0"s;
    string s2 = "b";

    ASSERT_EQ(&s1[0], test_strncat(&s1[0], &s2[0], s2.size()));
    ASSERT_EQ("ab"s, s1);
}

TEST_F(StrncatTest, short_cat) {
    string s1 = "a\0"s;
    string s2 = "bc";

    ASSERT_EQ(&s1[0], test_strncat(&s1[0], &s2[0], 1));
    ASSERT_EQ("ab"s, s1);
}
