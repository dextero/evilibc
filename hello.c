#include <stdio.h>

int main() {
    printf("0, default: %d\n"
           "1, width 3: %3d\n"
           "2, width 3, 0-padded: %03d\n"
           "0, precision 0: %.0d\n"
           "string: %s\n"
           "percent: %%\n",
           0, 1, 2, 0, "Hello evilibc!");
}
