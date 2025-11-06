#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

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

    signal(SIGUSR1,terminate_handler);
    while (1) {
        pause();
    }

    signal(SIG1, unsafe_handlers);
    return 0;
}

//testing signal safety, using an unsafe function in a signal handler
void unsafe_handlers(int signal) {
    printf("Terminating %d", signal);

    char *buff2 = malloc(50);

}
