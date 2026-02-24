#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main() {

    char *buffer1 = malloc(100);
    strcpy(buffer1, "Hello World");
    //Missing free(buffer1)

    char *buffer2 = malloc(200);
    if (buffer2 != NULL) {
        strcpy(buffer2, "Proper cleanup");
        free(buffer2);
    }

    int *numbers = malloc(10 * sizeof(int));
    numbers[0] = 42;
    //Missing free(numbers)

    //Potential double free
    char *temp = malloc(50);
    free(temp);

    int two = 2;
    char *intofl = malloc(50 * sizeof(two));
    free(intofl);
    // free(temp); safe

    signal(SIGUSR1,terminate_handler);
    while (1) {
        pause();
    }

    signal(SIG1, unsafe_handlers);

    char *doubleptr = malloc(10);
    free(doubleptr); free(doubleptr);
    return 0;
}

//testing signal safety, using an unsafe function in a signal handler
void unsafe_handlers(int signal) {
    printf("Terminating %d", signal);

    char *buff2 = malloc(50);

}

