#ifndef __EVIL_TEST_TEST_H
#define __EVIL_TEST_TEST_H

#include <gtest/gtest.h>

#include "UBChecker.h"

namespace evil {

class Test : public ::testing::Test
{
    UBChecker _disallow_ub;
};

} // namespace evil

#endif // __EVIL_TEST_TEST_H
