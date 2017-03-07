cmake_minimum_required(VERSION 2.8)

project(evilibc C)

set(CMAKE_C_FLAGS "-Wall -Wextra -nostdlib -nostdinc")

include_directories(include
                    src)
add_library(${PROJECT_NAME}
            src/ctype/ctype.c
            src/errno/errno.c
            src/internal/rand.c
            src/internal/undefined_behavior.c)

install(DIRECTORY include DESTINATION include/evilibc)
install(FILES "$<TARGET_FILE:${PROJECT_NAME}>" DESTINATION lib)
