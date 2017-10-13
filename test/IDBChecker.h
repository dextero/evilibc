#ifndef __EVIL_TEST_IDB_CHECKER_H
#define __EVIL_TEST_IDB_CHECKER_H

#include <cstddef>
#include <cstdio>

#include <vector>

#include <gtest/gtest.h>

#include "../include/evilibc.h"

namespace evil {

struct IDBHandlerContext
{
    evil_idb_handler_t *handler;
    void *handler_data;
};

class IDBChecker
{
    static std::vector<IDBHandlerContext> _handler_ctx_stack;
        
    size_t _idb_count = 0;
    size_t _expected_idb_count;

    static void update_handler()
    {
        __evil_set_idb_handler(_handler_ctx_stack.back().handler,
                               _handler_ctx_stack.back().handler_data);
    }

public:
    IDBChecker(size_t expected_idb_count = 0)
        : _expected_idb_count(expected_idb_count)
    {
        auto handler = [](void *self_,
                          const char *reason_fmt,
                          va_list reason_args) {
            IDBChecker *self = (IDBChecker *)self_;
            if (self->_idb_count >= self->_expected_idb_count) {
                fprintf(stderr, "unexpected IDB triggered: ");
                vfprintf(stderr, reason_fmt, reason_args);
            }

            EXPECT_LT(self->_idb_count, self->_expected_idb_count);
            ++self->_idb_count;
        };

        _handler_ctx_stack.emplace_back(IDBHandlerContext{handler, this});
        update_handler();
    }

    ~IDBChecker()
    {
        if (_idb_count != _expected_idb_count) {
            fprintf(stderr, "IDB hander was not called, but it should\n");
        }

        EXPECT_EQ(_expected_idb_count, _idb_count);
        _handler_ctx_stack.pop_back();
        update_handler();
    }
};

} // namespace evil

#endif // __EVIL_TEST_IDB_CHECKER_H
