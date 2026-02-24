#include "analyze.h"
#include "formatter.h"
#include <string.h>
#include <ctype.h>

ViolationNode* integer_overflow_rule(const SourceFile *file) {
    ViolationNode* violations = NULL;

    for (int i = 0; i < file->line_count; i++) {
        const char* line = file->lines[i];

        if ((strstr(line, "malloc") || strstr(line, "calloc") || strstr(line, "realloc"))) {
            if (strstr(line, "*") || strstr(line, "+") || strstr(line, "-")) {
                append_violation(
                    &violations, i + 1, line,
                    "Arithmetic operation detected inside allocation.",
                    INTEGER_OVERFLOW
                );
            }
        }

        if (strstr(line, "malloc") && strstr(line, "(int)")) {
            append_violation(
               &violations, i + 1, line,
               "Explicit cast to 'int' in allocation. Possible sign mismatch.",
               INTEGER_OVERFLOW
           );
        }

        if ((strstr(line, "argv") || strstr(line, "scanf") || strstr(line, "read(")) &&
            (strstr(line, "+") || strstr(line, "*"))) {
            append_violation(
                &violations, i + 1, line,
                "External input used in arithmetic without visible bounds check.",
                INTEGER_OVERFLOW
            );
            }
    }

    return violations;
}