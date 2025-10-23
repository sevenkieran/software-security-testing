#include "analyze.h"
#include "colors.h"
#include <stdio.h>
#include "formatter.h"

typedef struct {
    char *deprecated;
    char *replacement;
} HeaderReplacement;

static HeaderReplacement deprecated_headers[] = {
    {"varargs.h", "stdarg.h"},
    {"malloc.h", "stdlib.h"},
    {"memory.h", "string.h"},
    {"alloc.h", "stdlib.h"},
    {"values.h", "limits.h and float.h"},
    {NULL, NULL}
};

ViolationNode* deprecated_header_rule(const SourceFile *file) {
    ViolationNode*  violations = NULL;

    for (int i = 0; i < file->line_count; i++) {
        char *line = file->lines[i];
        char *trimmed = trim_line(line);

        if (strncmp(trimmed, "#include", 8) != 0) continue;

        for (int j = 0; deprecated_headers[j].deprecated != NULL; j++) {
            if (strstr(line, deprecated_headers[j].deprecated)) {

                char message[256];//for suggested replacement, can just put directly in message
                snprintf(message, sizeof(message),
                         "Deprecated header <%s> used. Consider replacing with <%s>.",
                         deprecated_headers[j].deprecated,
                         deprecated_headers[j].replacement);

                append_violation(&violations,
                                 i + 1,
                                 line,
                                 message,
                                 DEPRECATED_HEADER);
            }
        }
    }

    return violations;
}
