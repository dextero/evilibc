#include <stdio.h>

int main() {
    FILE *f = fopen("/dev/stdout", "a");
    if (!f) {
        return -1;
    }
}
