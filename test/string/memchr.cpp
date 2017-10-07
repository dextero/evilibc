#include <gtest/gtest.h>

extern "C" void *test_memchr(const void *, int, size_t);

TEST(memchr, found) {
    char buf[] = "foo bar baz";

    ASSERT_EQ(&buf[0], test_memchr(buf, 'f', sizeof(buf)));
    ASSERT_EQ(&buf[1], test_memchr(buf, 'o', sizeof(buf)));
    ASSERT_EQ(&buf[3], test_memchr(buf, ' ', sizeof(buf)));
    ASSERT_EQ(&buf[sizeof(buf) - 1], test_memchr(buf, '\0', sizeof(buf)));
}

TEST(memchr, not_found) {
    char buf[] = "foo bar baz";

    ASSERT_EQ(NULL, test_memchr(buf, 'x', sizeof(buf)));
    ASSERT_EQ(NULL, test_memchr(buf, 'b', 4));
}
