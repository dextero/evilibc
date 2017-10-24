#include "Test.h"

#include <unistd.h>

using namespace ::testing;
using namespace std;

extern "C" void __evil_init(void);
extern "C" void __evil_cleanup(void);

namespace evil {

void Test::EnableStandardIOStreams()
{
    evil::SyscallsMock syscalls;

    EXPECT_CALL(syscalls, _isatty(_))
        .Times(3)
        .WillRepeatedly(Return(1));

    EXPECT_CALL(syscalls, _access(_, _))
        .WillOnce(Return(0));

    EXPECT_CALL(syscalls, _open("/dev/stdin", _, _))
        .WillOnce(Return(0));
    EXPECT_CALL(syscalls, _open("/dev/stdout", _, _))
        .WillOnce(Return(1));
    EXPECT_CALL(syscalls, _open("/dev/stderr", _, _))
        .WillOnce(Return(2));

    __evil_init();

    EXPECT_CALL(_syscalls, _write(1, _, _))
        .WillRepeatedly(Invoke(write));
    EXPECT_CALL(_syscalls, _write(2, _, _))
        .WillRepeatedly(Invoke(write));
}

void Test::EnableHeap()
{
    _memory_pool = make_unique<SizedMemoryPool<1024 * 1024>>();

    EXPECT_CALL(_syscalls, _sbrk(_))
        .WillRepeatedly(Invoke(
                    [this](ptrdiff_t incr) {
                        return _memory_pool->sbrk(incr);
                    }));
}

void Test::SetUp()
{
    EnableStandardIOStreams();
    EnableHeap();
}

void Test::TearDown()
{
    evil::SyscallsMock syscalls;

    EXPECT_CALL(syscalls, _close(_))
        .WillRepeatedly(Return(0));
    EXPECT_CALL(syscalls, _write(_, _, _))
        .WillRepeatedly(ReturnArg<2>());

    __evil_cleanup();
}

} // namespace evil
