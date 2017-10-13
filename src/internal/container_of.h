#ifndef __EVILIBC_INTERNAL_CONTAINER_OF_H
#define __EVILIBC_INTERNAL_CONTAINER_OF_H

#define container_of(Ptr, Type, Member) \
    ((Type *)((char*)(Ptr) + (ptrdiff_t)(&(((Type *)NULL)->Member)) - offsetof(Type, Member)))

#endif /* __EVILIBC_INTERNAL_CONTAINER_OF_H */
