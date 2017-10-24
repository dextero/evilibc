#ifndef __EVIL_TEST_TEST_H
#define __EVIL_TEST_TEST_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

#include <memory>

#include "UBChecker.h"
#include "IDBChecker.h"
#include "SyscallsMock.h"
#include "MemoryPool.h"

namespace evil {

class Test : public ::testing::Test
{
    UBChecker _disallow_ub;
    IDBChecker _disallow_idb;
    std::unique_ptr<MemoryPool> _memory_pool;

protected:
    evil::SyscallsMock _syscalls;

    virtual void EnableStandardIOStreams();
    virtual void EnableHeap();

    virtual void SetUp();
    virtual void TearDown();
};

} // namespace evil

#endif // __EVIL_TEST_TEST_H
