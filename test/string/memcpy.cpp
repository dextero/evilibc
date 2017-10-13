#include "Test.h"

#include <string>

using namespace std;

extern "C" void *test_memcpy(void *, const void *, size_t n);

class MemcpyTest : public evil::Test {};

TEST_F(MemcpyTest, null) {
    {
        evil::UBChecker checker{1};
        ASSERT_EQ(NULL, test_memcpy(NULL, "", 0));
    }
    {
        evil::UBChecker checker{1};
        string buf = "";
        ASSERT_EQ(&buf[0], test_memcpy(&buf[0], NULL, 0));
    }
    {
        evil::UBChecker checker{1};
        ASSERT_EQ(NULL, test_memcpy(NULL, NULL, 0));
    }
}

TEST_F(MemcpyTest, overlapping_right) {
    evil::UBChecker checker{1};
    string buf = "1234";
    ASSERT_EQ(&buf[1], test_memcpy(&buf[1], &buf[0], buf.size() + 1));
    ASSERT_EQ("1234", buf);
}

TEST_F(MemcpyTest, overlapping_left) {
    evil::UBChecker checker{1};
    string buf = "1234";
    ASSERT_EQ(&buf[0], test_memcpy(&buf[0], &buf[1], buf.size() + 1));
    ASSERT_EQ("1234", buf);
}

TEST_F(MemcpyTest, basic) {
    string src = "12";
    string dst(5, '\0');

    ASSERT_EQ(&dst[0], test_memcpy(&dst[0], &src[0], dst.size()));
    ASSERT_EQ("12\0\0\0"s, dst);
}
