#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char* message;
    size_t line;
} Finding;

static const char* bad_headers[] = {
    "<iostream.h>", "<conio.h>", "<bits/stdc++.h>"
};
static const size_t bad_headers_count = sizeof(bad_headers) / sizeof(bad_headers[0]);

void analyze_deprecated_headers(const char* code, Finding* findings, size_t* finding_count, size_t max_findings) {
    size_t line_num = 1;
    const char* start = code;
    const char* ptr = code;

    *finding_count = 0;

    while (*ptr) {
        if (*ptr == '\n') {
            size_t line_len = ptr - start;
            char line[512];
            if (line_len >= sizeof(line))
                line_len = sizeof(line) - 1;

            strncpy(line, start, line_len);
            line[line_len] = '\0';

            for (size_t i = 0; i < bad_headers_count; i++) {
                char include_line[600];
                snprintf(include_line, sizeof(include_line), "#include %s", bad_headers[i]);
                if (strstr(line, include_line)) {
                    if (*finding_count < max_findings) {
                        findings[*finding_count].message = bad_headers[i];
                        findings[*finding_count].line = line_num;
                        (*finding_count)++;
                    }
                }
            }

            start = ptr + 1;
            line_num++;
        }
        ptr++;
    }
}
