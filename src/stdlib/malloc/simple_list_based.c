#include <evil-config.h>

#include "stdlib.h"

#include "assert.h"
#include "stdbool.h"
#define WITH_INTPTR_TYPES 1
#include "stdint.h"

#include "internal/container_of.h"
#include "internal/list.h"
#include "internal/undefined_behavior.h"

#include "os/syscalls.h"

#define MIN_CHUNK_SIZE ((size_t)4096)
#define ROUND_UP_TO_MULTIPLE_OF(N, Val) (((Val) + (N) - 1) / (N) * (N))

struct chunk {
    struct list list;
    size_t size;
    _Alignas(max_align_t) char data[];
};

static struct list *g_used_chunks = NULL;
static struct list *g_free_chunks = NULL;

static inline struct chunk *chunk_from_list(struct list *list) {
    return container_of(list, struct chunk, list);
}

static inline struct chunk *chunk_from_plist(struct list **plist) {
    return chunk_from_list(container_of(plist, struct list, next));
}

static struct chunk *alloc_new_chunk(size_t min_size)
{
    size_t alloc_size = ROUND_UP_TO_MULTIPLE_OF(MIN_CHUNK_SIZE, min_size);

    assert(alloc_size > 0);
    assert(alloc_size < PTRDIFF_MAX);

    void *p = _sbrk((int)alloc_size);
    if (!p) {
        return NULL;
    }

    struct chunk *chunk = (struct chunk *)p;
    chunk->size = alloc_size - sizeof(struct chunk);
    chunk->list.next = NULL;
    return chunk;
}

static bool chunk_bigger(struct list *list,
                         void *min_size)
{
    return chunk_from_list(list)->size >= *(size_t*)min_size;
}

static bool chunk_data_not_less(struct list *list,
                                void *ptr)
{
    return (uintptr_t)chunk_from_list(list)->data >= (uintptr_t)ptr;
}

static struct list **chunk_insert(struct list **plist,
                                  struct chunk *chunk)
{
    struct list **insert_ptr = list_find_ptr(plist,
                                             chunk_data_not_less, chunk->data);
    list_insert(insert_ptr, &chunk->list);
    return insert_ptr;
}

static struct list *get_free_chunk(size_t min_size)
{
    struct list **p = list_find_ptr(&g_free_chunks, chunk_bigger, &min_size);
    if (*p) {
        return *p;
    }

    struct chunk *chunk = alloc_new_chunk(min_size);
    if (!chunk) {
        return NULL;
    }

    chunk_insert(&g_free_chunks, chunk);
    return &chunk->list;
}

static struct chunk *chunk_split(struct chunk *chunk,
                                 size_t first_size)
{
    assert(chunk->size < sizeof(struct chunk));
    assert(first_size % _Alignof(struct chunk) == 0);

    struct chunk *second = (struct chunk *)&chunk->data[first_size];

    *second = (struct chunk) {
        .list = { .next = chunk->list.next },
        .size = chunk->size - sizeof(struct chunk) - first_size
    };
    *chunk = (struct chunk) {
        .list = { .next = &second->list },
        .size = first_size
    };
    return chunk;
}

static struct chunk *chunk_shrink_to_fit(struct chunk *chunk,
                                         size_t required_size)
{
    size_t min_size =
            ROUND_UP_TO_MULTIPLE_OF(_Alignof(struct chunk), required_size);

    if (chunk->size <= min_size) {
        return chunk;
    } else {
        return chunk_split(chunk, min_size);
    }
}

static struct list **chunk_find_used(void *p)
{
    struct list **plist = list_find_ptr(&g_used_chunks,
                                        chunk_data_not_less, p);
    if (!*plist) {
        return NULL;
    }

    if ((uintptr_t)chunk_from_list(*plist)->data != (uintptr_t)p) {
        return NULL;
    }

    return plist;
}

static void chunk_merge_with_next(struct chunk *first)
{
    assert(&first->data[first->size] == (char*)&first->list.next);

    struct chunk *second = chunk_from_list(first->list.next);
    first->list.next = second->list.next;
    first->size += second->size;
}

static void chunk_defragment(struct chunk *first)
{
    assert(first);

    while ((uintptr_t)&first->data[first->size]
               == (uintptr_t)first->list.next) {
        chunk_merge_with_next(first);
    }
}

void *malloc(size_t size)
{
    struct list *list = get_free_chunk(size);
    if (!list) {
        return NULL;
    }

    struct chunk *chunk =  chunk_shrink_to_fit(chunk_from_list(list), size);
    return chunk->data;
}

void free(void *p)
{
    if (p) {
        struct list **plist = chunk_find_used(p);
        if (!plist) {
            __evil_ub("invalid pointer passed to free(): %p", p);
        } else {
            struct list *elem = list_detach(plist);
            struct chunk *chunk = container_of(elem, struct chunk, list);
            struct list **prev = chunk_insert(&g_free_chunks, chunk);
            chunk_defragment(chunk_from_plist(prev));
        }
    }
}
