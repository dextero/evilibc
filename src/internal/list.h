#ifndef __EVILIBC_INTERNAL_LIST_H
#define __EVILIBC_INTERNAL_LIST_H

#include "assert.h"
#include "stdbool.h"

struct list {
    struct list *next;
};

static inline void list_insert(struct list **plist,
                               struct list *new_elem)
{
    assert(plist);
    assert(!new_elem->next);

    new_elem->next = *plist;
    *plist = new_elem;
}

static inline struct list *list_detach(struct list **plist)
{
    assert(plist);
    assert(*plist);

    struct list *curr = *plist;
    *plist = (*plist)->next;
    curr->next = NULL;
    return curr;
}

static inline struct list **list_find_ptr(struct list **plist,
                                          bool (*predicate)(struct list *list,
                                                            void *data),
                                          void *data)
{
    assert(plist);
    assert(predicate);

    for (; *plist; plist = &(*plist)->next) {
        if (predicate(*plist, data)) {
            break;
        }
    }

    return plist;
}

#endif /* __EVILIBC_INTERNAL_LIST_H */
