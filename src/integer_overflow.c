//
// Created by ryanb on 9/30/2025.
//
#include "analyze.h"
#include <string.h>

#include "colors.h"

int integer_overflow_rule(const SourceFile *file) {
    int violations = 0;
    for (int i = 0; i < file->line_count; i++) {
        char *line = file->lines[i];
        char *trimmed = trim_line(line);

        if (is_comment_or_preprocessor(line)) continue;

        // Check for multiplication in malloc/calloc without overflow check
        if ((strstr(line, "malloc(") || strstr(line, "calloc(")) && strstr(line, "*")) {
            violations++;
            printf(YEL"  Line %d: Potential integer overflow in allocation\n"reset, i + 1);
            printf(WHT"    Code: %s\n"reset, line);
        }

        // Check for array size calculations with addition
        if (strstr(line, "malloc(") && (strstr(line, " + ") || strstr(line, "++"))) {
            violations++;
            printf(YEL"  Line %d: Potential overflow in size calculation\n"reset, i + 1);
            printf(WHT"    Code: %s\n"reset, line);
        }
    }

    return violations;
}
