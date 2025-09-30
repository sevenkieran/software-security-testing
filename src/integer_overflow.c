//
// Created by ryanb on 9/30/2025.
//
#include "analyze.h"
#include <string.h>

int integer_overflow_rule(const SourceFile *file) {
    int violations = 0;
    for (int i = 0; i < file->line_count; i++) {
        char *line = file->lines[i];
        char *trimmed = trim_line(line);

        if (is_comment_or_preprocessor(line)) continue;

        // Check for multiplication in malloc/calloc without overflow check
        if ((strstr(line, "malloc(") || strstr(line, "calloc(")) && strstr(line, "*")) {
            violations++;
            printf("  Line %d: Potential integer overflow in allocation\n", i + 1);
            printf("    Code: %s\n", line);
        }

        // Check for array size calculations with addition
        if (strstr(line, "malloc(") && (strstr(line, " + ") || strstr(line, "++"))) {
            violations++;
            printf("  Line %d: Potential overflow in size calculation\n", i + 1);
            printf("    Code: %s\n", line);
        }
    }

    return violations;
}