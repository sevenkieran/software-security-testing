//
// Created by ryanb on 10/30/2025.
//

#include "../include/json_export.h"
#include <stdio.h>
#include<time.h>
#include <string.h>

void export_violations_json(const char *filename, int total_violations, const char* output_file) {
    FILE *f = fopen(output_file, "w");

    if (!f) {
        fprintf(stderr, "Cannot create JSON output file %s\n", output_file);
        return;
    }

    time_t currtime = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S", localtime(&currtime));

    //json format
    fprintf(f, "{\n");
    fprintf(f, "  \"timestamp\": \"%s\",\n", timestamp);
    fprintf(f, "  \"file_analyzed\": \"%s\",\n", filename);
    fprintf(f, "  \"total_violations\": %d,\n", total_violations);
    fprintf(f, "  \"status\": \"%s\"\n", total_violations == 0 ? "PASS" : "FAIL");
    fprintf(f, "}\n");

    fclose(f);

    printf("JSON report saved to: %s\n", output_file);
}