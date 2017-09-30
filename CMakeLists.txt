cmake_minimum_required(VERSION 2.8)

project(evilibc C)

set(CMAKE_C_FLAGS "-Wall -Wextra -nostdlib -nostdinc")

# GCC complains about that a lot
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-nonnull-compare")

include_directories(os/include
                    include)
add_subdirectory(os/linux-x86_64)
include_directories(SYSTEM
                    os/linux-x86_64/include
                    /usr/include/x86_64-linux-gnu)

include_directories(include
                    src)
add_library(${PROJECT_NAME}
            $<TARGET_OBJECTS:syscalls>
            src/ctype/ctype.c
            src/errno/errno.c
            src/stdio/fopen.c
            src/stdio/format.c
            src/stdio/fprintf.c
            src/stdio/fputc.c
            src/stdio/fwrite.c
            src/stdio/printf.c
            src/stdio/sprintf.c
            src/stdio/snprintf.c
            src/stdio/vfprintf.c
            src/stdio/vsprintf.c
            src/stdio/vsnprintf.c
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
            src/internal/init.c
            src/internal/memory.c
            src/internal/rand.c
            src/internal/undefined_behavior.c)

install(DIRECTORY include DESTINATION include/evilibc)
install(FILES "$<TARGET_FILE:${PROJECT_NAME}>" DESTINATION lib)

add_executable(hello hello.c)
target_link_libraries(hello ${PROJECT_NAME})
