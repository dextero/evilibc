add_executable(test_stdlib_malloc
               stdlib/malloc.c
               $<TARGET_OBJECTS:syscalls>)
target_link_libraries(test_stdlib_malloc evilibc)
