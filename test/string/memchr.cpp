#include "Test.h"

extern "C" void *test_memchr(const void *, int, size_t);

class MemchrTest : public evil::Test {};

TEST_F(MemchrTest, found) {
    char buf[] = "foo bar baz";

    ASSERT_EQ(&buf[0], test_memchr(buf, 'f', sizeof(buf)));
    ASSERT_EQ(&buf[1], test_memchr(buf, 'o', sizeof(buf)));
    ASSERT_EQ(&buf[3], test_memchr(buf, ' ', sizeof(buf)));
    ASSERT_EQ(&buf[sizeof(buf) - 1], test_memchr(buf, '\0', sizeof(buf)));
}

TEST_F(MemchrTest, not_found) {
    char buf[] = "foo bar baz";

    ASSERT_EQ(NULL, test_memchr(buf, 'x', sizeof(buf)));
    ASSERT_EQ(NULL, test_memchr(buf, 'b', 4));
}

TEST_F(MemchrTest, zero) {
    ASSERT_EQ(NULL, test_memchr(" ", ' ', 0));

    {
        auto checker = evil::UBChecker{1};
        ASSERT_EQ(NULL, test_memchr(NULL, ' ', 0));
    }
}

TEST_F(MemchrTest, null) {
    auto checker = evil::UBChecker{1};
    ASSERT_EQ(NULL, test_memchr(NULL, ' ', 1));
}
