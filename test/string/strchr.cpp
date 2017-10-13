#include "Test.h"

#include <string>
#include <limits>

using namespace std;

extern "C" char *test_strchr(const char *, int c);

class StrchrTest : public evil::Test {};

TEST_F(StrchrTest, null_str) {
    evil::UBChecker checker{1};
    ASSERT_EQ(NULL, test_strchr(NULL, ' '));
}

TEST_F(StrchrTest, invalid_char) {
#ifndef __CHAR_UNSIGNED__
    {
        evil::IDBChecker checker{1};
        ASSERT_EQ(NULL, test_strchr("", numeric_limits<char>::min() - 1));
    }
    {
        evil::IDBChecker checker{1};
        ASSERT_EQ(NULL, test_strchr("", numeric_limits<char>::max() + 1));
    }
#endif // __CHAR_UNSIGNED__
}

TEST_F(StrchrTest, no_occurrences) {
    string s = "a";
    ASSERT_EQ(NULL, test_strchr(&s[0], 'b'));
}

TEST_F(StrchrTest, single_occurrence) {
    string s = "abc";
    ASSERT_EQ(&s[1], test_strchr(&s[0], 'b'));
}

TEST_F(StrchrTest, multiple_occurrence) {
    string s = "aba";
    ASSERT_EQ(&s[0], test_strchr(&s[0], 'a'));
}

TEST_F(StrchrTest, nullbyte) {
    string s = "a";
    ASSERT_EQ(&s[1], test_strchr(&s[0], '\0'));
}
