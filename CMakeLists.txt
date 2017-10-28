cmake_minimum_required(VERSION 3.5)

project(evilibc C CXX)
set(evilibc_VERSION 0.1)

option(WITH_POSIX "Enable POSIX features" OFF)
option(WITH_XSI "Enable XSI features" OFF)

option(WITH_FIXED_WIDTH_TYPES "Support optional intN_t/uintN_t types" OFF)
option(WITH_64BIT_FIXED_WIDTH_TYPES "Support optional int64_t/uint64_t types" OFF)
option(WITH_INTPTR_TYPES "Support intptr_t/uintptr_t types" OFF)

option(WITH_FLOATING_POINT_TIME_T "Define time_t as a floating-point type" ON)

option(WITH_USABLE_MALLOC "Prevent malloc/calloc/realloc from leaking memory (see malloc implemnetation for details)" ON)
option(WITH_DETERMINISTIC_SETVBUF "Do not interpret MAY in setvbuf() spec literally" ON)

set(WARNING_FLAGS
    -W
    -Wall
    -Wextra
    -Wcast-align
    -Wcast-qual
    -Wdouble-promotion
    -Wduplicated-cond
    -Werror=address
    -Werror=aggressive-loop-optimizations
    -Werror=array-bounds
    -Werror=char-subscripts
    -Werror=empty-body
    -Werror=enum-compare
    -Werror=float-equal
    -Werror=ignored-qualifiers
    -Werror=implicit-int
    -Werror=init-self
    -Werror=logical-op
    -Werror=main
    -Werror=missing-braces
    -Werror=missing-parameter-type
    -Werror=nonnull
    -Werror=old-style-declaration
    -Werror=parentheses
    -Werror=pointer-sign
    -Werror=return-type
    -Werror=sequence-point
    -Werror=uninitialized
    -Werror=volatile-register-var
    -Werror=write-strings
    -Wformat=2
    -Wformat-signedness
    -Wframe-larger-than=32768
    -Wjump-misses-init
    -Wlogical-op
    -Wnull-dereference
    -Wodr
    -Wshadow
    -Wshift-overflow=2
    -Wsign-compare
    -Wstrict-aliasing=2
    -Wsuggest-final-methods
    -Wsuggest-final-types
    -Wsync-nand
    -Wtrampolines
    -Wuninitialized
    CACHE LIST "Warning flags to use if available")

add_subdirectory(os/stub)
add_subdirectory(os/linux-x86_64)

configure_file(config/evil-config.h.in
               "${CMAKE_CURRENT_BINARY_DIR}/config/evil-config.h"
               @ONLY)

set(PUBLIC_INCLUDES
    include/assert.h
    include/ctype.h
    include/errno.h
    include/evilibc.h
    include/inttypes.h
    include/limits.h
    include/math.h
    include/signal.h
    include/stdarg.h
    include/stdbool.h
    include/stddef.h
    include/stdint.h
    include/stdio.h
    include/stdlib.h
    include/string.h
    include/time.h
    )

add_library(evilibc
            ${PUBLIC_INCLUDES}
            src/ctype/ctype.c
            src/errno/errno.c
            src/evilibc.c
            src/internal/container_of.h
            src/internal/init.c
            src/internal/list.h
            src/internal/memory.c
            src/internal/memory.h
            src/internal/rand.c
            src/internal/rand.h
            src/internal/undefined_behavior.c
            src/internal/undefined_behavior.h
            src/signal/raise.c
            src/stdio/fclose.c
            src/stdio/ferror.c
            src/stdio/fflush.c
            src/stdio/fopen.c
            src/stdio/format.c
            src/stdio/format.h
            src/stdio/fprintf.c
            src/stdio/fputc.c
            src/stdio/fwrite.c
            src/stdio/internal/file.h
            src/stdio/internal/utils.c
            src/stdio/internal/utils.h
            src/stdio/printf.c
            src/stdio/setbuf.c
            src/stdio/setvbuf.c
            src/stdio/snprintf.c
            src/stdio/sprintf.c
            src/stdio/vfprintf.c
            src/stdio/vsnprintf.c
            src/stdio/vsprintf.c
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
            src/string/strcspn.c
            src/string/strerror.c
            src/string/strlen.c
            src/string/strncat.c
            src/string/strncmp.c
            src/string/strncpy.c
            src/string/strpbrk.c
            src/string/strrchr.c
            src/string/strspn.c
            src/string/strstr.c
            src/string/strtok.c
            )

target_include_directories(evilibc PRIVATE
                           src)
target_include_directories(evilibc PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/config>
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                           $<INSTALL_INTERFACE:include/evilibc>)

target_compile_options(evilibc PRIVATE
                       ${WARNING_FLAGS})

target_compile_options(evilibc PUBLIC
                       -nostdlib -nostdinc)

target_compile_features(evilibc PUBLIC
                          c_function_prototypes
                          c_restrict
                        PRIVATE
                          c_static_assert)

target_link_libraries(evilibc PUBLIC
                      runtime)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/evilibc-version.cmake"
                                 VERSION ${evilibc_VERSION}
                                 COMPATIBILITY SameMajorVersion)

install(FILES
        "evilibc-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/evilibc-version.cmake"
        DESTINATION lib/cmake/evilibc)
install(FILES ${PUBLIC_INCLUDES}
        DESTINATION include/evilibc)
install(TARGETS evilibc EXPORT evilibc_targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include/evilibc)
install(EXPORT evilibc_targets
        FILE evilibc_targets.cmake
        NAMESPACE evilibc::
        DESTINATION lib/cmake/evilibc)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Evil libc implementation")
set(CPACK_PACKAGE_VENDOR "dextero")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
include(CPack)

enable_testing()
add_subdirectory(test EXCLUDE_FROM_ALL)

add_custom_target(check COMMAND env GTEST_COLOR=1 ${CMAKE_CTEST_COMMAND} --verbose)
add_dependencies(check all_tests)

