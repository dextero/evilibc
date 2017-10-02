#define WITH_INTPTR_TYPES 1
#include "stdint.h"
#include "stdlib.h"

#include "assert.h"

#include "internal/memory.h"

static char mem_block[4096 * 2];

void *_sbrk(ptrdiff_t increment) {
    static char *brk = mem_block;

    char *new_brk = brk + increment;
    if (new_brk > mem_block + sizeof(mem_block)) {
        return NULL;
    }

    char *old_brk = brk;
    brk = new_brk;
    return old_brk;
}

static const size_t sizes[] = { 16, 16, 32, 64, 128 };

static inline size_t size_for_index(size_t i) {
    return sizes[i % (sizeof(sizes) / sizeof(sizes[0]))];
}

int main() {
    void *ptrs[64] = {0};
    for (size_t i = 0; i < sizeof(ptrs) / sizeof(ptrs[0]); ++i) {
        ptrs[i] = malloc(size_for_index(i));
        assert(ptrs[i]);

        for (size_t j = 0; j < i; ++j) {
            assert(!__evil_regions_overlap(ptrs[i], size_for_index(i),
                                           ptrs[j], size_for_index(j)));
        }
    }

    for (size_t i = 0; i < sizeof(ptrs) / sizeof(ptrs[0]); ++i) {
        free(ptrs[i]);
    }

    void *c = malloc(8000);
    assert(c);
    free(c);

    void *d = malloc(8192);
    assert(d == NULL);
    free(NULL);

    return EXIT_SUCCESS;
}
