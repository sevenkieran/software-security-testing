#include "analyze.h"

int memory_leak_rule(const SourceFile *file) {
    int violations = 0;
    int malloc_count = 0;
    int free_count = 0;

    printf("Checking for memory management issues...\n");

    for (int i = 0; i < file->line_count; i++) {
        char *line = file->lines[i];

        //skip comments and preprocessor directives
        if (is_comment_or_preprocessor(line)) continue;

        // Check for memory allocation
        if (strstr(line, "malloc(") || strstr(line, "calloc(") || strstr(line, "realloc(")) {
            malloc_count++;
            printf("Line %d: Memory allocation - %s\n", i + 1, line);
        }

        // Check for memory deallocation
        if (strstr(line, "free(")) {
            free_count++;
        }

        // Check for obvious double free
        if (strstr(line, "free(") && strstr(line, "free(") != strrchr(line, 'f')) {
            violations++;
            printf("Line %d: Potential double free - %s\n", i + 1, line);
        }

        // Check for null pointer dereference after malloc
        if (strstr(line, "malloc(") && i + 1 < file->line_count) {
            char *next_line = file->lines[i + 1];
            if (!is_comment_or_preprocessor(next_line) &&
                !strstr(next_line, "if") && !strstr(next_line, "NULL") && !strstr(next_line, "==")) {
                violations++;
                printf("Line %d: malloc() not checked for NULL - %s\n", i + 1, line);
                }
        }
    }

    printf("Memory allocations: %d, frees: %d\n", malloc_count, free_count);

    // Simple heuristic: more mallocs than frees suggests potential leak
    if (malloc_count > free_count) {
        violations += (malloc_count - free_count);
        printf("Warning: %d more allocations than frees (potential memory leak)\n",
               malloc_count - free_count);
    }

    return violations;
}