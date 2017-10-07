string(APPEND CMAKE_C_FLAGS " -std=gnu11 -fno-stack-protector")

include_directories(include)
add_library(runtime STATIC
            src/syscalls.c
            src/startup.c)
target_include_directories(runtime SYSTEM PUBLIC
                           ../include
                           /usr/include/x86_64-linux-gnu)
