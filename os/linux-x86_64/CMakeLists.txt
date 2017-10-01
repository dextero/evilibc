include_directories(SYSTEM
                    include
                    /usr/include/x86_64-linux-gnu)

add_library(syscalls OBJECT
            src/syscalls.c
            src/startup.c)
