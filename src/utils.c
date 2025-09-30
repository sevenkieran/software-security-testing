#include <string.h>

#include "utils.h"

bool is_c_or_h_file(const char *path) {
    const char *dot = strrchr(path, '.');
    if (!dot) return 0;
    return (strcmp(dot, ".c") == 0 || strcmp(dot, ".h") == 0);
}