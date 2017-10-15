#include "Test.h"

#include <string>

using namespace std;

extern "C" int test_snprintf(char *, size_t, const char *, ...);

class SnprintfTest : public evil::Test {};

TEST_F(SnprintfTest, no_args) {
    char dst[sizeof("abc")];

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "abc"));
    EXPECT_EQ("abc"s, string(dst));
}

TEST_F(SnprintfTest, format_invalid) {
    char dst[sizeof("%?")];
    evil::UBChecker checker{1};

    EXPECT_EQ(2, test_snprintf(dst, sizeof(dst), "%?"));
    EXPECT_EQ("%?"s, string(dst));
}

TEST_F(SnprintfTest, format_string) {
    char dst[16];

    EXPECT_EQ(5, test_snprintf(dst, sizeof(dst), "AB%s", "abc"));
    EXPECT_EQ("ABabc"s, string(dst));

    EXPECT_EQ(5, test_snprintf(dst, sizeof(dst), "A%sB", "abc"));
    EXPECT_EQ("AabcB"s, string(dst));

    EXPECT_EQ(5, test_snprintf(dst, sizeof(dst), "%sAB", "abc"));
    EXPECT_EQ("abcAB"s, string(dst));

    {
        evil::UBChecker checker{1};

        EXPECT_EQ(6, test_snprintf(dst, sizeof(dst), "%s", NULL));
        EXPECT_EQ("(null)"s, string(dst));
    }
}

TEST_F(SnprintfTest, width) {
    char dst[16];

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "% 2s", "abc"));
    EXPECT_EQ("abc"s, string(dst));

    EXPECT_EQ(5, test_snprintf(dst, sizeof(dst), "% 5s", "abc"));
    EXPECT_EQ("  abc"s, string(dst));

    EXPECT_EQ(5, test_snprintf(dst, sizeof(dst), "%-5s", "abc"));
    EXPECT_EQ("abc  "s, string(dst));

    EXPECT_EQ(5, test_snprintf(dst, sizeof(dst), "% *s", 5, "abc"));
    EXPECT_EQ("  abc"s, string(dst));

    EXPECT_EQ(5, test_snprintf(dst, sizeof(dst), "%*s", -5, "abc"));
    EXPECT_EQ("abc  "s, string(dst));
}

TEST_F(SnprintfTest, precision) {
    char dst[16];

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%.3s", "abcde"));
    EXPECT_EQ("abc"s, string(dst));

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%.5s", "abc"));
    EXPECT_EQ("abc"s, string(dst));

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%.*s", 3, "abcde"));
    EXPECT_EQ("abc"s, string(dst));

    EXPECT_EQ(5, test_snprintf(dst, sizeof(dst), "%.*s", -3, "abcde"));
    EXPECT_EQ("abcde"s, string(dst));
}

TEST_F(SnprintfTest, width_and_precision) {
    char dst[16];

    EXPECT_EQ(5, test_snprintf(dst, sizeof(dst), "% 5.3s", "abcde"));
    EXPECT_EQ("  abc"s, string(dst));

    EXPECT_EQ(5, test_snprintf(dst, sizeof(dst), "% *.*s", 5, 3, "abcde"));
    EXPECT_EQ("  abc"s, string(dst));
}

TEST_F(SnprintfTest, format_char) {
    char dst[sizeof("xAB")];

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "AB%c", 'a'));
    EXPECT_EQ("ABa"s, string(dst));

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "A%cB", 'a'));
    EXPECT_EQ("AaB"s, string(dst));

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%cAB", 'a'));
    EXPECT_EQ("aAB"s, string(dst));
}

TEST_F(SnprintfTest, format_char_outside_range) {
    char dst[sizeof("A")];

    EXPECT_EQ(1, test_snprintf(dst, sizeof(dst), "%c", 'A' - 256));
    EXPECT_EQ("A"s, string(dst));

    EXPECT_EQ(1, test_snprintf(dst, sizeof(dst), "%c", 'A' + 256));
    EXPECT_EQ("A"s, string(dst));
}

TEST_F(SnprintfTest, format_int_decimal) {
    char dst[16];

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%d", 123));
    EXPECT_EQ("123"s, string(dst));

    EXPECT_EQ(4, test_snprintf(dst, sizeof(dst), "%d", -123));
    EXPECT_EQ("-123"s, string(dst));
}

TEST_F(SnprintfTest, format_int_decimal_prefixes) {
    char dst[16];

    EXPECT_EQ(4, test_snprintf(dst, sizeof(dst), "%+d", 123));
    EXPECT_EQ("+123"s, string(dst));

    EXPECT_EQ(4, test_snprintf(dst, sizeof(dst), "%+d", -123));
    EXPECT_EQ("-123"s, string(dst));

    EXPECT_EQ(4, test_snprintf(dst, sizeof(dst), "% d", 123));
    EXPECT_EQ(" 123"s, string(dst));

    EXPECT_EQ(4, test_snprintf(dst, sizeof(dst), "% d", -123));
    EXPECT_EQ("-123"s, string(dst));
}

TEST_F(SnprintfTest, format_int_zero_pad) {
    char dst[16];

    EXPECT_EQ(8, test_snprintf(dst, sizeof(dst), "%08d", 123));
    EXPECT_EQ("00000123"s, string(dst));

    EXPECT_EQ(8, test_snprintf(dst, sizeof(dst), "%08d", -123));
    EXPECT_EQ("-0000123"s, string(dst));
}

TEST_F(SnprintfTest, format_unsigned_decimal) {
    char dst[16];

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%u", 123));
    EXPECT_EQ("123"s, string(dst));
}

TEST_F(SnprintfTest, format_unsigned_hex) {
    char dst[16];

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%x", 0x1af));
    EXPECT_EQ("1af"s, string(dst));

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%X", 0x1AF));
    EXPECT_EQ("1AF"s, string(dst));
}

TEST_F(SnprintfTest, format_unsigned_hex_alternative) {
    char dst[16];

    EXPECT_EQ(5, test_snprintf(dst, sizeof(dst), "%#x", 0x1af));
    EXPECT_EQ("0x1af"s, string(dst));

    EXPECT_EQ(5, test_snprintf(dst, sizeof(dst), "%#X", 0x1AF));
    EXPECT_EQ("0X1AF"s, string(dst));
}

TEST_F(SnprintfTest, format_unsigned_hex_alternative_zero_pad) {
    char dst[16];

    EXPECT_EQ(8, test_snprintf(dst, sizeof(dst), "%#08x", 0x1af));
    EXPECT_EQ("0x0001af"s, string(dst));

    EXPECT_EQ(8, test_snprintf(dst, sizeof(dst), "%#08X", 0x1AF));
    EXPECT_EQ("0X0001AF"s, string(dst));
}
