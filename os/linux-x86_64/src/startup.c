#include "os/syscalls.h"

void __evil_init(void);
int main(int argc, char *argv[], char *envp[]);

void _start(void) {
    __evil_init();
    _exit(main(0, 0, 0));
}
