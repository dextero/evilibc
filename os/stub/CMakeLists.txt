add_library(runtime_stub STATIC
            src/startup.c)

target_compile_features(runtime_stub PUBLIC
                          c_function_prototypes)

target_include_directories(runtime_stub PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/../include)
