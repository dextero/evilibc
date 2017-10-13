#include "evilibc.h"

extern evil_ub_handler_t *__evil_ub_handler;
extern void *__evil_ub_handler_data;

extern evil_ub_handler_t *__evil_idb_handler;
extern void *__evil_idb_handler_data;

void __evil_set_ub_handler(evil_ub_handler_t *handler,
                           void *data)
{
    __evil_ub_handler = handler;
    __evil_ub_handler_data = data;
}

void __evil_set_idb_handler(evil_idb_handler_t *handler,
                            void *data)
{
    __evil_idb_handler = handler;
    __evil_idb_handler_data = data;
}

