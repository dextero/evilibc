cmake_minimum_required(VERSION 2.8)

project(evilibc C CXX)

# GCC complains a lot about redundant != NULL checks
set(CMAKE_C_FLAGS "-Wall -Wextra -std=c11 -Wno-nonnull-compare" CACHE STRING "")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++14" CACHE STRING "")

option(WITH_POSIX "Enable POSIX features" OFF)
option(WITH_XSI "Enable XSI features" OFF)

option(WITH_FIXED_WIDTH_TYPES "Support optional intN_t/uintN_t types" OFF)
option(WITH_64BIT_FIXED_WIDTH_TYPES "Support optional int64_t/uint64_t types" OFF)
option(WITH_INTPTR_TYPES "Support intptr_t/uintptr_t types" OFF)

option(WITH_USABLE_MALLOC "Prevent malloc/calloc/realloc from leaking memory (see malloc implemnetation for details)" ON)
option(WITH_DETERMINISTIC_SETVBUF "Do not interpret MAY in setvbuf() spec literally" ON)

configure_file(config/evil-config.h.in
               "${CMAKE_CURRENT_BINARY_DIR}/config/evil-config.h"
               @ONLY)
include_directories("${CMAKE_CURRENT_BINARY_DIR}/config")

add_library(evilibc
            include/stdio.h
            include/ctype.h
            include/stdbool.h
            include/evilibc.h
            include/stdarg.h
            include/signal.h
            include/time.h
            include/stdlib.h
            include/limits.h
            include/string.h
            include/assert.h
            include/errno.h
            include/stddef.h
            include/stdint.h
            src/evilibc.c
            src/internal/memory.h
            src/internal/list.h
            src/internal/undefined_behavior.c
            src/internal/rand.c
            src/internal/syscall.h
            src/internal/rand.h
            src/internal/container_of.h
            src/internal/init.c
            src/internal/memory.c
            src/internal/undefined_behavior.h
            src/errno/errno.c
            src/stdlib/malloc/simple_list_based.c
            src/stdlib/abort.c
            src/signal/raise.c
            src/string/strtok.c
            src/string/strerror.c
            src/string/strlen.c
            src/string/memmove.c
            src/string/memcpy.c
            src/string/strncat.c
            src/string/strcspn.c
            src/string/strpbrk.c
            src/string/strchr.c
            src/string/strspn.c
            src/string/strrchr.c
            src/string/strcpy.c
            src/string/strcat.c
            src/string/strncpy.c
            src/string/memset.c
            src/string/strstr.c
            src/string/strcmp.c
            src/string/memcmp.c
            src/string/memchr.c
            src/string/strncmp.c
            src/ctype/ctype.c
            src/stdio/fputc.c
            src/stdio/fopen.c
            src/stdio/printf.c
            src/stdio/fflush.c
            src/stdio/internal/file.h
            src/stdio/internal/utils.h
            src/stdio/internal/utils.c
            src/stdio/fwrite.c
            src/stdio/setbuf.c
            src/stdio/sprintf.c
            src/stdio/vsprintf.c
            src/stdio/vsnprintf.c
            src/stdio/setvbuf.c
            src/stdio/snprintf.c
            src/stdio/format.c
            src/stdio/format.h
            src/stdio/vfprintf.c
            src/stdio/fprintf.c
            src/stdio/fclose.c)

set(EVILIBC_C_FLAGS
    -nostdlib -nostdinc
    -I${CMAKE_CURRENT_SOURCE_DIR}/include
    -I${CMAKE_CURRENT_SOURCE_DIR}/os/include
    -I${CMAKE_CURRENT_SOURCE_DIR}/os/linux-x86_64/include
    -I${CMAKE_CURRENT_SOURCE_DIR}/src
    -isystem /usr/include/x86_64-linux-gnu)
set_target_properties(evilibc PROPERTIES
                      COMPILE_OPTIONS "${EVILIBC_C_FLAGS}")

install(DIRECTORY include DESTINATION include/evilibc)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/config/evil-config.h" DESTINATION include/evilibc/include)
install(FILES "$<TARGET_FILE:evilibc>" DESTINATION lib)

add_subdirectory(os/linux-x86_64)

enable_testing()
add_subdirectory(test)

add_custom_target(check COMMAND env GTEST_COLOR=1 ${CMAKE_CTEST_COMMAND} --verbose)
add_dependencies(check all_tests)

