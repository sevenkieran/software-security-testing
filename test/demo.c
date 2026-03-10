#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    printf("Testing memory leak detection\n");

    // Memory leak example
    char *buffer1 = malloc(100);
    strcpy(buffer1, "Hello World");
    // Missing free(buffer1) - should trigger leak warning
}