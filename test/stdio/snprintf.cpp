#include "Test.h"

#include <string>

using namespace std;
using namespace ::testing;

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
    {
        evil::UBChecker checker{1};
        EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%hs", "abc"));
        EXPECT_EQ("%hs"s, string(dst));
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

    EXPECT_EQ(0, test_snprintf(dst, sizeof(dst), "%.s", "abcde"));
    EXPECT_EQ(""s, string(dst));

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

    {
        evil::UBChecker checker{1};
        EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%hc", 'a'));
        EXPECT_EQ("%hc"s, string(dst));
    }
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

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%hhd", 123 + UINT8_MAX + 1));
    EXPECT_EQ("123"s, string(dst));

    EXPECT_EQ(4, test_snprintf(dst, sizeof(dst), "%hhd", -123 - UINT8_MAX - 1));
    EXPECT_EQ("-123"s, string(dst));

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%hd", 123 + UINT16_MAX + 1));
    EXPECT_EQ("123"s, string(dst));

    EXPECT_EQ(4, test_snprintf(dst, sizeof(dst), "%hd", -123 - UINT16_MAX - 1));
    EXPECT_EQ("-123"s, string(dst));

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%d", 123));
    EXPECT_EQ("123"s, string(dst));

    EXPECT_EQ(4, test_snprintf(dst, sizeof(dst), "%d", -123));
    EXPECT_EQ("-123"s, string(dst));

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%ld", 123L));
    EXPECT_EQ("123"s, string(dst));

    EXPECT_EQ(4, test_snprintf(dst, sizeof(dst), "%ld", -123L));
    EXPECT_EQ("-123"s, string(dst));

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%lld", 123LL));
    EXPECT_EQ("123"s, string(dst));

    EXPECT_EQ(4, test_snprintf(dst, sizeof(dst), "%lld", -123LL));
    EXPECT_EQ("-123"s, string(dst));

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%zd", (ssize_t)123));
    EXPECT_EQ("123"s, string(dst));

    EXPECT_EQ(4, test_snprintf(dst, sizeof(dst), "%zd", (ssize_t)-123));
    EXPECT_EQ("-123"s, string(dst));

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%jd", (intmax_t)123));
    EXPECT_EQ("123"s, string(dst));

    EXPECT_EQ(4, test_snprintf(dst, sizeof(dst), "%jd", (intmax_t)-123));
    EXPECT_EQ("-123"s, string(dst));

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%td", (ptrdiff_t)123));
    EXPECT_EQ("123"s, string(dst));

    EXPECT_EQ(4, test_snprintf(dst, sizeof(dst), "%td", (ptrdiff_t)-123));
    EXPECT_EQ("-123"s, string(dst));

    {
        evil::UBChecker checker{1};
        EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%Ld", 123));
        EXPECT_EQ("%Ld"s, dst);
    }

    {
        evil::UBChecker checker{1};
        EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%#d", 123));
        EXPECT_EQ("%#d"s, dst);
    }
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

TEST_F(SnprintfTest, format_int_precision) {
    char dst[16];

    EXPECT_EQ(8, test_snprintf(dst, sizeof(dst), "%.8d", 123));
    EXPECT_EQ("00000123"s, string(dst));

    EXPECT_EQ(9, test_snprintf(dst, sizeof(dst), "%.8d", -123));
    EXPECT_EQ("-00000123"s, string(dst));
}

TEST_F(SnprintfTest, format_unsigned_decimal) {
    char dst[16];

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%hhu", 123U + UINT8_MAX + 1U));
    EXPECT_EQ("123"s, string(dst));

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%hu", 123U + UINT16_MAX + 1U));
    EXPECT_EQ("123"s, string(dst));

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%u", 123U));
    EXPECT_EQ("123"s, string(dst));

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%lu", 123L));
    EXPECT_EQ("123"s, string(dst));

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%llu", 123LL));
    EXPECT_EQ("123"s, string(dst));

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%zd", (size_t)123));
    EXPECT_EQ("123"s, string(dst));

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%jd", (uintmax_t)123));
    EXPECT_EQ("123"s, string(dst));

    {
        evil::UBChecker checker{1};
        EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%tu", (ptrdiff_t)123));
        EXPECT_EQ("%tu"s, string(dst));
    }
    {
        evil::UBChecker checker{1};
        EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%Lu", 123U));
        EXPECT_EQ("%Lu"s, dst);
    }
    {
        evil::UBChecker checker{1};
        EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%#u", 123U));
        EXPECT_EQ("%#u"s, dst);
    }
}

TEST_F(SnprintfTest, format_unsigned_octal) {
    char dst[16];

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%o", 0123));
    EXPECT_EQ("123"s, string(dst));
}

TEST_F(SnprintfTest, format_unsigned_octal_alternative) {
    char dst[16];

    EXPECT_EQ(4, test_snprintf(dst, sizeof(dst), "%#o", 0123));
    EXPECT_EQ("0123"s, string(dst));
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

TEST_F(SnprintfTest, pointer) {
    char dst[64];

    EXPECT_LT(0, test_snprintf(dst, sizeof(dst), "%p", (void *)dst));
    EXPECT_THAT(string(dst), MatchesRegex("^[A-Za-z]+$"));

    {
        evil::UBChecker checker{1};
        EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%lp", (void *)dst));
        EXPECT_EQ("%lp"s, string(dst));
    }
}

TEST_F(SnprintfTest, bytes_written) {
    char dst[64];
    int written;

    EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "%s%nc", "ab", &written));
    EXPECT_EQ("abc"s, string(dst));
    EXPECT_EQ(2, written);

    {
        evil::UBChecker checker{1};
        EXPECT_EQ(6, test_snprintf(dst, sizeof(dst), "%s% nc", "ab"));
        EXPECT_EQ("ab% nc"s, string(dst));
    }
    {
        evil::UBChecker checker{1};
        EXPECT_EQ(6, test_snprintf(dst, sizeof(dst), "%s%8nc", "ab"));
        EXPECT_EQ("ab%8nc"s, string(dst));
    }
    {
        evil::UBChecker checker{1};
        EXPECT_EQ(7, test_snprintf(dst, sizeof(dst), "%s%.8nc", "ab"));
        EXPECT_EQ("ab%.8nc"s, string(dst));
    }
    {
        evil::UBChecker checker{1};
        EXPECT_EQ(6, test_snprintf(dst, sizeof(dst), "%s%hnc", "ab"));
        EXPECT_EQ("ab%hnc"s, string(dst));
    }
}

TEST_F(SnprintfTest, literal_percent) {
    char dst[64];

    EXPECT_EQ(1, test_snprintf(dst, sizeof(dst), "%%"));
    EXPECT_EQ("%"s, string(dst));

    {
        evil::UBChecker checker{1};
        EXPECT_EQ(3, test_snprintf(dst, sizeof(dst), "% %"));
        EXPECT_EQ("% %"s, string(dst));
    }
    {
        evil::UBChecker checker{1};
        EXPECT_EQ(6, test_snprintf(dst, sizeof(dst), "%s%8%c", "ab"));
        EXPECT_EQ("ab%8%c"s, string(dst));
    }
    {
        evil::UBChecker checker{1};
        EXPECT_EQ(7, test_snprintf(dst, sizeof(dst), "%s%.8%c", "ab"));
        EXPECT_EQ("ab%.8%c"s, string(dst));
    }
    {
        evil::UBChecker checker{1};
        EXPECT_EQ(6, test_snprintf(dst, sizeof(dst), "%s%h%c", "ab"));
        EXPECT_EQ("ab%h%c"s, string(dst));
    }
}

TEST_F(SnprintfTest, conversion_limit) {
    char dst[4096];
    char src[8192];
    memset(src, 'A', sizeof(src));
    src[sizeof(src) - 1] = '\0';

    {
        evil::IDBChecker checker{1};
        EXPECT_EQ(4095, test_snprintf(dst, sizeof(dst), "%s", src));
        EXPECT_EQ(string(4095, 'A'), string(dst));
    }
    {
        evil::IDBChecker checker{1};
        EXPECT_EQ(4095, test_snprintf(dst, sizeof(dst), "% 8192s", "A"));
        EXPECT_EQ(string(4094, ' ') + "A", string(dst));
    }
    {
        evil::IDBChecker checker{1};
        EXPECT_EQ(4095, test_snprintf(dst, sizeof(dst), "%.8192s", src));
        EXPECT_EQ(string(4095, 'A'), string(dst));
    }
}
