#include <stdio.h>
#include <string.h>

int main(void) {
    char small[8];
    char big[] = "This string is definitely longer than eight bytes.";

    // ❗ Overflow: big is larger than small, but strcpy does not check sizes
    strcpy(small, big);

    printf("small = %s\n", small);
    return 0;
}
