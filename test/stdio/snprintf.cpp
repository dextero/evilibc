#include "Test.h"

#include <string>

using namespace std;

extern "C" int test_snprintf(char *, size_t, const char *, ...);

class SnprintfTest : public evil::Test {};

TEST_F(SnprintfTest, no_args) {
    char dst[sizeof("abc")];

    ASSERT_EQ(3, test_snprintf(dst, sizeof(dst), "abc"));
    ASSERT_EQ("abc"s, string(dst));
}

TEST_F(SnprintfTest, format_invalid) {
    char dst[sizeof("%?")];
    evil::UBChecker checker{1};

    ASSERT_EQ(2, test_snprintf(dst, sizeof(dst), "%?"));
    ASSERT_EQ("%?"s, string(dst));
}

TEST_F(SnprintfTest, format_string) {
    char dst[sizeof("abcAB")];

    ASSERT_EQ(5, test_snprintf(dst, sizeof(dst), "AB%s", "abc"));
    ASSERT_EQ("ABabc"s, string(dst));

    ASSERT_EQ(5, test_snprintf(dst, sizeof(dst), "A%sB", "abc"));
    ASSERT_EQ("AabcB"s, string(dst));

    ASSERT_EQ(5, test_snprintf(dst, sizeof(dst), "%sAB", "abc"));
    ASSERT_EQ("abcAB"s, string(dst));
}

TEST_F(SnprintfTest, format_char) {
    char dst[sizeof("xAB")];

    ASSERT_EQ(3, test_snprintf(dst, sizeof(dst), "AB%c", 'a'));
    ASSERT_EQ("ABa"s, string(dst));

    ASSERT_EQ(3, test_snprintf(dst, sizeof(dst), "A%cB", 'a'));
    ASSERT_EQ("AaB"s, string(dst));

    ASSERT_EQ(3, test_snprintf(dst, sizeof(dst), "%cAB", 'a'));
    ASSERT_EQ("aAB"s, string(dst));
}

TEST_F(SnprintfTest, format_char_outside_range) {
    char dst[sizeof("A")];

    ASSERT_EQ(1, test_snprintf(dst, sizeof(dst), "%c", 'A' - 256));
    ASSERT_EQ("A"s, string(dst));

    ASSERT_EQ(1, test_snprintf(dst, sizeof(dst), "%c", 'A' + 256));
    ASSERT_EQ("A"s, string(dst));
}

TEST_F(SnprintfTest, format_int) {
    char dst[16];

    ASSERT_EQ(3, test_snprintf(dst, sizeof(dst), "%d", 123));
    ASSERT_EQ("123"s, string(dst));

    ASSERT_EQ(4, test_snprintf(dst, sizeof(dst), "%d", -123));
    ASSERT_EQ("-123"s, string(dst));
}
