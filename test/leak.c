#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

int main() {
    printf("Testing memory leak detection\n");

    // Memory leak example
    char *buffer1 = malloc(100);
    strcpy(buffer1, "Hello World");
    // Missing free(buffer1) - should trigger leak warning

    // Proper memory management
    char *buffer2 = malloc(200);
    if (buffer2 != NULL) {
        strcpy(buffer2, "Proper cleanup");
        free(buffer2);
    }

    // Another leak
    int *numbers = malloc(10 * sizeof(int));
    numbers[0] = 42;
    // Missing free(numbers) - should trigger leak warning

    // Potential double free (commented out to avoid crash)
    char *temp = malloc(50);
    free(temp);

    int two = 2;
    char *intofl = malloc(50 * sizeof(two));
    free(intofl);
    // free(temp);  // Would be double free if uncommented

    return 0;
}