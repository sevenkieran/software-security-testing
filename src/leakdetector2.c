//
// Created by ryanb on 9/11/2025.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structure for findings
typedef struct {
    const char* message;
    size_t line;
} Finding;

void analyze_leaks(const char* code, Finding* findings, size_t* finding_count, size_t max_findings) {
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

            // very simple check: malloc without free
            if (strstr(line, "malloc(")) {
                if (*finding_count < max_findings) {
                    findings[*finding_count].message = "Potential malloc without free";
                    findings[*finding_count].line = line_num;
                    (*finding_count)++;
                }
            }

            start = ptr + 1;
            line_num++;
        }
        ptr++;
    }
}
