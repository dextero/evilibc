#include <stdio.h>

int main() {
    FILE *f = fopen("/dev/stdout", "a");
    if (!f) {
        return -1;
    }

    const char hello[] = "Hello evilibc!\n";
    return (int)fwrite(hello, sizeof(hello), 1, f);
}
