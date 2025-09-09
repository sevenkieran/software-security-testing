#include <stdlib.h>

int main() {
    char *a = malloc(100);
    char *b = malloc(50);
    free(b);
    return 0;
}