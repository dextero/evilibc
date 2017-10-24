#include "os/syscalls.h"

void __evil_init(void);
void __evil_cleanup(void);
int main(int argc, char *argv[], char *envp[]);

void _start(void) {
    __evil_init();
    int result = main(0, 0, 0);
    __evil_cleanup();
    _exit(result);
}
