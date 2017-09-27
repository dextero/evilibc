#include "os/syscalls.h"

int main(int argc, char *argv[], char *envp[]);

void _start(void) {
    _exit(main(0, 0, 0));
}
