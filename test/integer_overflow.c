#include <stdio.h>
#include <limits.h>
#include <malloc.h>

int main(void) {
    int a = INT_MAX;
    int b = 1;

    char* buffer = malloc(100);
    free(buffer);
    int result = a + b;

    printf("a      = %d\n", a);
    printf("b      = %d\n", b);
    printf("result = %d\n", result);

    return 0;
}
