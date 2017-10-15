#include "Test.h"

#include <array>
#include <algorithm>

#include "internal/rand.h"

using namespace std;
using namespace ::testing;

class RandTest : public evil::Test {};

TEST_F(RandTest, uniform_enough) {
    array<size_t, UINT16_MAX + 1> buckets{};

    for (size_t i = 0 ; i < (UINT16_MAX * 1000);  ++i) {
        ++buckets[__evil_rand() % (UINT16_MAX + 1)];
    }

    /* arbitrarily chosen, +-150 */
    auto minmax = minmax_element(begin(buckets), end(buckets));
    ASSERT_LT(*minmax.second - *minmax.first, (size_t)300);
}

TEST_F(RandTest, rand_range) {
    for (size_t i = 0; i < 1024; ++i) {
        auto min_max = minmax({__evil_rand(), __evil_rand()});
        ASSERT_NE(min_max.first, min_max.second);

        int val = __evil_rand_range(min_max.first, min_max.second);
        ASSERT_LE(min_max.first, val);
        ASSERT_LT(val, min_max.second);
    }
}

TEST_F(RandTest, rand_negative) {
    for (size_t i = 0; i < 1024; ++i) {
        ASSERT_GT(0, __evil_rand_negative());
    }
}

TEST_F(RandTest, rand_positive) {
    for (size_t i = 0; i < 1024; ++i) {
        ASSERT_LT(0, __evil_rand_positive());
    }
}

TEST_F(RandTest, rand_nonzero) {
    for (size_t i = 0; i < 1024; ++i) {
        ASSERT_NE(0, __evil_rand_nonzero());
    }
}

TEST_F(RandTest, rand_with_sign) {
    for (size_t i = 0; i < 1024; ++i) {
        ASSERT_GT(0, __evil_rand_with_sign(__evil_rand_negative()));
    }
    for (size_t i = 0; i < 1024; ++i) {
        ASSERT_LT(0, __evil_rand_with_sign(__evil_rand_positive()));
    }
    ASSERT_EQ(0, __evil_rand_with_sign(0));
}
