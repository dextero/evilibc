cmake_minimum_required(VERSION 2.8)

project(evilibc C)

set(CMAKE_C_FLAGS "-Wall -Wextra -nostdlib -nostdinc")

# GCC complains about that a lot
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-nonnull-compare")

include_directories(include
                    src)
add_library(${PROJECT_NAME}
            src/ctype/ctype.c
            src/errno/errno.c
            src/stdio/fopen.c
            src/string/memchr.c
            src/string/memcmp.c
            src/string/memcpy.c
            src/string/memmove.c
            src/string/memset.c
            src/string/strcat.c
            src/string/strchr.c
            src/string/strcmp.c
            src/string/strcpy.c
            src/string/strerror.c
            src/string/strlen.c
            src/string/strncat.c
            src/string/strncmp.c
            src/string/strncpy.c
            src/string/strpbrk.c
            src/string/strrchr.c
            src/string/strcspn.c
            src/string/strstr.c
            src/string/strspn.c
            src/string/strtok.c
            src/internal/memory.c
            src/internal/rand.c
            src/internal/undefined_behavior.c)

install(DIRECTORY include DESTINATION include/evilibc)
install(FILES "$<TARGET_FILE:${PROJECT_NAME}>" DESTINATION lib)
