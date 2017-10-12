set(GTEST_PREFIX "${CMAKE_BINARY_DIR}")
set(GTEST_INSTALL_DIR "${GTEST_PREFIX}/install")

include(ExternalProject)
ExternalProject_Add(googletest
                    GIT_REPOSITORY https://github.com/google/googletest
                    CMAKE_ARGS
                        "-DCMAKE_INSTALL_PREFIX=${GTEST_INSTALL_DIR}"
                        "-DCMAKE_C_FLAGS=-std=c++11"
                    PREFIX "${GTEST_PREFIX}")

set(GTEST_INCLUDE_DIRS ${GTEST_INSTALL_DIR}/include)
set(GTEST_LIBRARY_DIRS ${GTEST_INSTALL_DIR}/lib)
set(GTEST_LIBRARIES gtest gtest_main gmock pthread)

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
add_dependencies(evilibc_test googletest)

add_library(evilibc_test_utils
            UBChecker.cpp
            UBChecker.h
            IDBChecker.cpp
            IDBChecker.h
            Test.h)
add_dependencies(evilibc_test_utils googletest)

include_directories(${GTEST_INCLUDE_DIRS})
link_directories(${GTEST_LIBRARY_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
link_libraries(${GTEST_LIBRARIES} evilibc_test runtime evilibc_test_utils)

add_executable(all_tests
               string/memchr.cpp
               string/memmove.cpp
               string/memcmp.cpp
               string/strchr.cpp
               string/strcpy.cpp
               string/strncpy.cpp
               string/strcspn.cpp
               string/strcat.cpp
               string/strncat.cpp
               string/strcmp.cpp
               string/strncmp.cpp
               string/strrchr.cpp
               string/strspn.cpp
               string/strstr.cpp
               string/strpbrk.cpp
               string/strtok.cpp)

add_test(NAME all COMMAND $<TARGET_FILE:all_tests>)
