#include "Test.h"

#include <string>

using namespace std;

extern "C" void *test_memmove(void *, const void *, size_t);

class MemmoveTest : public evil::Test {};

static const char TEST_VAL[] = "1234";

TEST_F(MemmoveTest, non_overlapping) {
    string src = "1234";
    string dst = "    ";

    ASSERT_EQ(&dst[0], test_memmove(&dst[0], &src[0], 1));
    ASSERT_EQ("1   ", dst);

    ASSERT_EQ(&dst[0], test_memmove(&dst[0], &src[0], src.size()));
    ASSERT_EQ(src, dst);
}

TEST_F(MemmoveTest, overlapping_right) {
    string buf = "1234";

    ASSERT_EQ(&buf[1], test_memmove(&buf[1], &buf[0], buf.size() - 1));
    ASSERT_EQ("1123", buf);
}

TEST_F(MemmoveTest, overlapping_left) {
    string buf = "1234";

    ASSERT_EQ(&buf[0], test_memmove(&buf[0], &buf[1], buf.size() - 1));
    ASSERT_EQ("2344", buf);
}

TEST_F(MemmoveTest, null) {
    string buf = "0";

    {
        evil::UBChecker checker{1};
        ASSERT_EQ(NULL, test_memmove(NULL, &buf[0], 1));
    }

    {
        evil::UBChecker checker{1};
        ASSERT_EQ(NULL, test_memmove(&buf[0], NULL, 1));
    }

    {
        evil::UBChecker checker{1};
        ASSERT_EQ(NULL, test_memmove(NULL, NULL, 1));
    }
}

TEST_F(MemmoveTest, zero) {
    string src = "1";
    string dst = "0";

    ASSERT_EQ(&dst[0], test_memmove(&dst[0], &src[0], 0));
    ASSERT_EQ(0, memcmp(&dst[0], "0", 2));

    {
        evil::UBChecker checker{1};
        ASSERT_EQ(NULL, test_memmove(NULL, &src[0], 0));
    }

    {
        evil::UBChecker checker{1};
        ASSERT_EQ(NULL, test_memmove(&dst[0], NULL, 0));
    }

    {
        evil::UBChecker checker{1};
        ASSERT_EQ(NULL, test_memmove(NULL, NULL, 0));
    }
}
