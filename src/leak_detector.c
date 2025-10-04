#include "analyze.h"
#include "colors.h"

int memory_leak_rule(const SourceFile *file) {
    int violations = 0;
    int malloc_count = 0;
    int free_count = 0;

    for (int i = 0; i < file->line_count; i++) {
        char *line = file->lines[i];

        //skip comments and preprocessor directives
        if (is_comment_or_preprocessor(line)) continue;

        // Check for memory allocation
        if (strstr(line, "malloc(") || strstr(line, "calloc(") || strstr(line, "realloc(")) {
            malloc_count++;
            printf(GRN"Line %d: Memory allocation - %s\n"reset, i + 1, line);
        }

        // Check for memory deallocation
        if (strstr(line, "free(")) {
            free_count++;
        }

        // Check for obvious double free
        if (strstr(line, "free(") && strstr(line, "free(") != strrchr(line, 'f')) {
            violations++;
            printf(RED"Line %d: Potential double free - %s\n"reset, i + 1, line);
        }

        // Check for null pointer dereference after malloc
        if (strstr(line, "malloc(") && i + 1 < file->line_count) {
            char *next_line = file->lines[i + 1];
            if (!is_comment_or_preprocessor(next_line) &&
                !strstr(next_line, "if") && !strstr(next_line, "NULL") && !strstr(next_line, "==")) {
                violations++;
                printf(YEL"Line %d: malloc() not checked for NULL - %s\n"reset, i + 1, line);
                }
        }
    }

    printf(BWHT"Memory allocations: %d, frees: %d\n"reset, malloc_count, free_count);

    // Simple heuristic: more mallocs than frees suggests potential leak
    if (malloc_count > free_count) {
        violations += (malloc_count - free_count);
        printf(BYEL"Warning: %d more allocations than frees (potential memory leak)\n"reset,
               malloc_count - free_count);
    }

    return violations;
}