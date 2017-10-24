add_subdirectory(googletest)

function(add_prefix_to_all_symbols TARGET_NAME PREFIX IN_LIBRARY_TARGET)
    set(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/lib${TARGET_NAME}.a")

    add_custom_target("generate_${TARGET_NAME}")
    add_custom_command(TARGET "generate_${TARGET_NAME}" PRE_BUILD
                       COMMAND cp "$<TARGET_FILE:${IN_LIBRARY_TARGET}>" "${OUTPUT}"
                       COMMAND objdump -t "${OUTPUT}"
                               | awk '$$2 == \"g\" && $$3 ~ /.*F.*/ { print $$6 \" test_\" $$6 }'
                               | grep -v __evil
                               > ${OUTPUT}.renames
                       COMMAND objcopy --redefine-syms="${OUTPUT}.renames" "${OUTPUT}"
                       MAIN_DEPENDENCY "${IN_LIBRARY_TARGET}")

    add_library("${TARGET_NAME}" STATIC IMPORTED)
    set_target_properties("${TARGET_NAME}" PROPERTIES IMPORTED_LOCATION "${OUTPUT}")
    add_dependencies("${TARGET_NAME}" "generate_${TARGET_NAME}")
endfunction()

add_prefix_to_all_symbols(evilibc_test test_ evilibc)

add_library(evilibc_test_utils
            UBChecker.cpp
            UBChecker.h
            IDBChecker.cpp
            IDBChecker.h
            SyscallsMock.cpp
            SyscallsMock.h
            Test.cpp
            Test.h)
target_link_libraries(evilibc_test_utils evilibc_test runtime_stub gtest gmock)
target_include_directories(evilibc_test_utils INTERFACE
                           "${CMAKE_CURRENT_SOURCE_DIR}"
                           "${CMAKE_CURRENT_SOURCE_DIR}/../src")

add_executable(all_tests
               internal/rand.cpp
               stdio/fclose.cpp
               stdio/fflush.cpp
               stdio/fopen.cpp
               stdio/sprintf.cpp
               stdio/snprintf.cpp
               stdio/vsprintf.cpp
               stdlib/malloc/simple_list_based.cpp
               string/memchr.cpp
               string/memcmp.cpp
               string/memcpy.cpp
               string/memmove.cpp
               string/memset.cpp
               string/strcat.cpp
               string/strchr.cpp
               string/strcmp.cpp
               string/strcpy.cpp
               string/strcspn.cpp
               string/strerror.cpp
               string/strlen.cpp
               string/strncat.cpp
               string/strncmp.cpp
               string/strncpy.cpp
               string/strpbrk.cpp
               string/strrchr.cpp
               string/strspn.cpp
               string/strstr.cpp
               string/strtok.cpp)
target_link_libraries(all_tests evilibc_test_utils gmock_main)

add_test(NAME all COMMAND $<TARGET_FILE:all_tests>)
