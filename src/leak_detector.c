#include "analyze.h"
#include "formatter.h"
#include <string.h>

memory_leak_rule(const SourceFile *file) {
    ViolationNode* violations = NULL;

    int malloc_count = 0;
    int free_count = 0;

    for (int i = 0; i < file->line_count; i++) {
        char *line = file->lines[i];

        if (is_comment_or_preprocessor(line)) continue;

        // Check for memory allocation
        if (strstr(line, "malloc(") || strstr(line, "calloc(") || strstr(line, "realloc(")) {
            malloc_count++;
            append_violation(&violations,
                           i + 1,
                           line,
                           "Memory allocation found",
                           MEMORY_LEAK);
        }

        // Check for free
        if (strstr(line, "free(")) {
            free_count++;
        }

        // Check for double free
        if (strstr(line, "free(") && strstr(line, "free(") != strrchr(line, 'f')) {
            append_violation(&violations,
                           i + 1,
                           line,
                           "Potential double free detected",
                           MEMORY_LEAK);
        }
    }

    // Overall leak check
    if (malloc_count > free_count) {
        char message[256];
        snprintf(message, sizeof(message),
                "Potential memory leak: %d allocations vs %d frees",
                malloc_count, free_count);

        append_violation(&violations,
                       file->line_count,
                       "",
                       message,
                       MEMORY_LEAK);
    }

    return violations;
}