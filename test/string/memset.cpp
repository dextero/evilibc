#include "Test.h"

#include <string>

using namespace std;

extern "C" void *test_memset(void *, int, size_t);

class MemsetTest : public evil::Test {};

TEST_F(MemsetTest, null) {
    {
        evil::UBChecker checker{1};
        ASSERT_EQ(NULL, test_memset(NULL, 0, 0));
    }
}

TEST_F(MemsetTest, basic) {
    string buf = "1234";
    ASSERT_EQ(&buf[0], test_memset(&buf[0], 0, 0));
    ASSERT_EQ("1234", buf);
    ASSERT_EQ(&buf[0], test_memset(&buf[0], 'A', 1));
    ASSERT_EQ("A234", buf);
    ASSERT_EQ(&buf[0], test_memset(&buf[0], 'B', buf.size()));
    ASSERT_EQ("BBBB", buf);
}
