#include <evil-config.h>

#include "stdlib.h"

#include "assert.h"
#include "stdbool.h"
#define WITH_INTPTR_TYPES 1
#include "stdint.h"
#include "string.h"

#include "internal/container_of.h"
#include "internal/list.h"
#include "internal/undefined_behavior.h"
#include "internal/rand.h"

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

void __evil_malloc_reset(void) {
    g_used_chunks = NULL;
    g_free_chunks = NULL;
}

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
    chunk->size = alloc_size - offsetof(struct chunk, data);
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
                                  struct list *chunk)
{
    struct list **insert_ptr = list_find_ptr(plist, chunk_data_not_less,
                                             chunk_from_list(chunk)->data);
    list_insert(insert_ptr, chunk);
    return insert_ptr;
}

static struct list **get_free_chunk(size_t min_size)
{
    struct list **p = list_find_ptr(&g_free_chunks, chunk_bigger, &min_size);
    if (*p) {
        return p;
    }

    struct chunk *chunk = alloc_new_chunk(min_size);
    if (!chunk) {
        return NULL;
    }

    return chunk_insert(&g_free_chunks, &chunk->list);
}

static struct chunk *chunk_split(struct chunk *chunk,
                                 size_t first_size)
{
    assert(chunk->size > offsetof(struct chunk, data));
    assert(first_size % _Alignof(struct chunk) == 0);

    struct chunk *second = (struct chunk *)&chunk->data[first_size];

    *second = (struct chunk) {
        .list = { .next = chunk->list.next },
        .size = chunk->size - offsetof(struct chunk, data) - first_size
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
    assert(&first->data[first->size]
            == (char*)chunk_from_list(first->list.next));

    struct chunk *second = chunk_from_list(first->list.next);
    first->list.next = second->list.next;
    first->size += offsetof(struct chunk, data) + second->size;
}

static void chunk_defragment(struct chunk *first)
{
    assert(first);

    while ((uintptr_t)&first->data[first->size]
               == (uintptr_t)chunk_from_list(first->list.next)) {
        chunk_merge_with_next(first);
    }
}

#define EVIL_POINTER (void*)0x1

static void *allocate(size_t size)
{
    // TODO: synchronization
    if (size == 0) {
        /*
         * 7.22.3, 1:
         * > If the size of the space requested is zero, the behavior is
         * > implementation-defined: either a null pointer is returned, or
         * > the behavior is as if the size were some nonzero value, except
         * > that the returned pointer shall not be used to access an object.
         */
        __evil_idb("malloc(0) may return NULL or some non-NULL value. If it "
                   "returns non-NULL, returned value MUST NOT be dereferenced.");
        return EVIL_POINTER;
    }

    struct list **list = get_free_chunk(size);
    if (!list) {
        return NULL;
    }

    struct chunk *chunk =  chunk_shrink_to_fit(chunk_from_list(*list), size);
    chunk_insert(&g_used_chunks, list_detach(list));

    return chunk->data;
}

void *malloc(size_t size)
{
    void *mem = allocate(size);
    /*
     * 7.22.3.4, 3:
     * > The malloc function returns either a null pointer or a pointer to
     * > the allocated space.
     *
     * Well, if the spec says so...
     */
#if WITH_USABLE_MALLOC
    return mem;
#else // WITH_USABLE_MALLOC
    return __evil_rand_bool() ? mem : NULL;
#endif // WITH_USABLE_MALLOC
}

void *calloc(size_t nmemb,
             size_t size)
{
    if (nmemb && SIZE_MAX / nmemb <= size) {
        return NULL;
    }

    /*
     * 7.22.3.2, 3:
     * > The calloc function allocates space for an array of *nmemb* objects,
     * > each of whose size is *size*. The space is initialized to all bits
     * > zero.
     */
    void *mem = malloc(nmemb * size);
    if (mem) {
        memset(mem, 0, nmemb * size);
    }

    /*
     * 7.22.3.2, 3:
     * > The calloc function returns either a null pointer or a pointer to
     * > the allocated space.
     *
     * The evilness already done in malloc().
     */
    return mem;
}

void *realloc(void *ptr,
              size_t size)
{
    if (!ptr || ptr == EVIL_POINTER) {
        /*
         * 7.22.3.5, 3:
         * > If ptr is a null pointer, the realloc function behaves like the
         * > malloc function for the specified size.
         */
        return malloc(size);
    }

    struct list **plist = chunk_find_used(ptr);
    if (!plist) {
        /*
         * 7.22.3.5, 3:
         * > If ptr does not match a pointer earlier returned by a memory
         * > management function, or if the space has been deallocated by a
         * > call to the free or realloc function, the behavior is undefined.
         */
        __evil_ub("invalid pointer passed to realloc(): %p", ptr);
        return NULL;
    } else {
        /*
         * That's a *very* lazy implementation.
         * TODO: check for possible memory extension first.
         */
        void *new_mem = allocate(size);
        if (!new_mem) {
            /*
             * 7.22.3.5, 3:
             * > If memory for the new object cannot be allocated, the old
             * > object is not deallocated and its value is unchanged.
             */
            return NULL;
        }

        struct chunk *chunk = chunk_from_list(*plist);
        memcpy(new_mem, ptr, chunk->size < size ? chunk->size : size);

        free(ptr);

        /*
         * 7.22.3.5, 4:
         * > The realloc function returns a pointer to the new object (which
         * > may have the same value as a pointer to the old object), or a
         * > null pointer if the new object could not be allocated.
         *
         * Finally, an allocation function that gets its return value right.
         * It's a shame @p ptr needs to be non-NULL for that to work.
         */
        return new_mem;
    }
}

void free(void *p)
{
    /*
     * 7.22.3.3, 1:
     * > If ptr is a null pointer, no action occurs.
     */
    if (p && p != EVIL_POINTER) {
        struct list **plist = chunk_find_used(p);
        if (!plist) {
            /*
             * 7.22.3.3, 2:
             * > if the argument does not match a pointer earlier returned by
             * > a memory management function, or if the space has been
             * > deallocated by a call to free or realloc, the behavior is
             * > undefined.
             *
             * TODO: check which functions are defined as "memory allocation
             * functions"
             */
            __evil_ub("invalid pointer passed to free(): %p", p);
        } else {
            struct list *elem = list_detach(plist);
            struct list **prev = chunk_insert(&g_free_chunks, elem);

            if (prev == &g_free_chunks) {
                chunk_defragment(chunk_from_list(*prev));
            } else {
                chunk_defragment(chunk_from_plist(prev));
            }
        }
    }
}
