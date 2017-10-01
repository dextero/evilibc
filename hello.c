#include <stdio.h>

int main() {
    printf("0, default: %d\n"
           "1, width 3: %3d\n"
           "2, width 3, 0-padded: %03d\n"
           "0, precision 0: %.0d\n"
           "4294967296, long, signed: %ld\n"
           "4294967296, long, unsigned: %ld\n"
           "4294967296, long, octal: %lo\n"
           "4294967296, long, hex: %lx\n"
           "4294967296, long, HEX: %lX\n"
           "'x', char: %c\n"
           "string: %s\n"
           "pointer: %p\n"
           "percent: %%\n",
           0, 1, 2, 0, 4294967296L,
           4294967296UL, 4294967296UL, 4294967296UL, 4294967296UL,
           'x', "Hello evilibc!", &(int){0});
}
