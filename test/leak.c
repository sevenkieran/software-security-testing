#include <stdlib.h>
#include <stdio.h>

void badcode_test() {
    char *b = malloc(50);
    free(b);
    //
    //
    int x;
    printf("Value of x: %d\n", x); // undefined garbage
}