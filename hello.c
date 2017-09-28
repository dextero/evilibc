#include <stdio.h>

int main() {
    const char hello[] = "Hello evilibc!\n";
    return (int)fwrite(hello, sizeof(hello), 1, stdout);
}
