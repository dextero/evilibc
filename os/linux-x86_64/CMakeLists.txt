add_library(runtime STATIC
            src/syscalls.c
            src/startup.c)

target_compile_options(runtime PRIVATE
                       -std=gnu11 ${WARNING_FLAGS})

target_compile_features(runtime PUBLIC
                          c_function_prototypes
                        PRIVATE
                          c_static_assert
                          c_variadic_macros)

target_include_directories(runtime SYSTEM PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_include_directories(runtime PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
                           $<INSTALL_INTERFACE:../include>)

install(TARGETS runtime EXPORT runtime_targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include)
install(EXPORT runtime_targets
        FILE runtime_targets.cmake
        NAMESPACE runtime::
        DESTINATION lib/cmake/evilibc_runtime)
