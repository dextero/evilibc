cmake_minimum_required(VERSION 2.8)

project(evilibc C CXX)

# GCC complains a lot about redundant != NULL checks
set(CMAKE_C_FLAGS "-Wall -Wextra -std=c11 -Wno-nonnull-compare")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++14")

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
            src/ctype/ctype.c
            src/errno/errno.c
            src/signal/raise.c
            src/stdio/internal/utils.c
            src/stdio/fclose.c
            src/stdio/fflush.c
            src/stdio/fopen.c
            src/stdio/format.c
            src/stdio/fprintf.c
            src/stdio/fputc.c
            src/stdio/fwrite.c
            src/stdio/setbuf.c
            src/stdio/setvbuf.c
            src/stdio/printf.c
            src/stdio/sprintf.c
            src/stdio/snprintf.c
            src/stdio/vfprintf.c
            src/stdio/vsprintf.c
            src/stdio/vsnprintf.c
            src/stdlib/abort.c
            src/stdlib/malloc/simple_list_based.c
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
