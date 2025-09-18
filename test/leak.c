#include <stdlib.h>
#include <stdio.h>

int mainn() {
    char *a = malloc(100);
    char *b = malloc(50);
    free(b);


    int x;
    printf("Value of x: %d\n", x); // undefined garbage


    return 0;
}