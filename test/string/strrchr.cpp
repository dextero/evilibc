#include "Test.h"

#include <string>
#include <limits>

using namespace std;

extern "C" char *test_strrchr(const char *, int c);

class StrrchrTest : public evil::Test {};

TEST_F(StrrchrTest, null_str) {
    evil::UBChecker checker{1};
    ASSERT_EQ(NULL, test_strrchr(NULL, ' '));
}

TEST_F(StrrchrTest, invalid_char) {
#ifndef __CHAR_UNSIGNED__
    {
        evil::IDBChecker checker{1};
        ASSERT_EQ(NULL, test_strrchr("", numeric_limits<char>::min() - 1));
    }
    {
        evil::IDBChecker checker{1};
        ASSERT_EQ(NULL, test_strrchr("", numeric_limits<char>::max() + 1));
    }
#endif // __CHAR_UNSIGNED__
}

TEST_F(StrrchrTest, no_occurrences) {
    string s = "a";
    ASSERT_EQ(NULL, test_strrchr(&s[0], 'b'));
}

TEST_F(StrrchrTest, single_occurrence) {
    string s = "abc";
    ASSERT_EQ(&s[1], test_strrchr(&s[0], 'b'));
}

TEST_F(StrrchrTest, multiple_occurrence) {
    string s = "aba";
    ASSERT_EQ(&s[2], test_strrchr(&s[0], 'a'));
}

TEST_F(StrrchrTest, nullbyte) {
    string s = "a";
    ASSERT_EQ(&s[1], test_strrchr(&s[0], '\0'));
}
