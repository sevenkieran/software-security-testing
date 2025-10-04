#include "analyze.h"
#include "colors.h"

// List of deprecated headers and their replacements
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

int deprecated_header_rule(const SourceFile *file) {
    int violations = 0;

    printf(BWHT"Checking for deprecated headers...\n"reset);

    for (int i = 0; i < file->line_count; i++) {
        char *line = file->lines[i];
        char *trimmed = trim_line(line);

        // Only check #include lines
        if (strncmp(trimmed, "#include", 8) != 0) continue;

        // Check against each deprecated header
        for (int j = 0; deprecated_headers[j].deprecated != NULL; j++) {
            if (strstr(line, deprecated_headers[j].deprecated)) {
                violations++;
                printf(YEL"      Line %d: Deprecated header '%s' found\n"reset,
                       i + 1, deprecated_headers[j].deprecated);
                printf(CYN"      Suggested: Use #include <%s>\n"reset,
                       deprecated_headers[j].replacement);
                printf(WHT"      Line: %s\n"reset, line);
            }
        }
    }

    return violations;
}