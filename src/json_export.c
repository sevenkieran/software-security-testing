//
// Created by ryanb on 10/30/2025.
//

#include "../include/json_export.h"
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <string.h>
#include "colors.h"
void append_file_result(FileResultNode** head, const char* filename, int violation_count) {
    FileResultNode* newNode = (FileResultNode*)malloc(sizeof(FileResultNode));
    if (!newNode) {
        fprintf(stderr, "%sError: Could not allocate memory for file result.\n%s", BRED, CLRreset);
        return;
    }
    newNode->filename = strdup(filename);
    if (!newNode->filename) {
        fprintf(stderr, "%sError: Could not duplicate filename string.\n%s", BRED, CLRreset);
        free(newNode);
        return;
    }
    newNode->violation_count = violation_count;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
        return;
    }
    FileResultNode* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}
void free_file_results(FileResultNode* head) {
    FileResultNode* current = head;
    FileResultNode* next_node;
    while (current != NULL) {
        next_node = current->next;
        free(current->filename);
        free(current);
        current = next_node;
    }
}

void export_violations_json(const char* analysis_path,
                            int total_violations,
                            const FileResultNode* head,
                            const char* output_file) {
    FILE* f = fopen(output_file, "w");
    if (!f) {
        fprintf(stderr, "Cannot create JSON output file %s\n", output_file);
        return;
    }

    time_t currtime = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S", localtime(&currtime));

    fprintf(f, "{\n");
    fprintf(f, "  \"timestamp\": \"%s\",\n", timestamp);
    fprintf(f, "  \"analysis_path\": \"%s\",\n", analysis_path); // Use the analysis path
    fprintf(f, "  \"total_violations\": %d,\n", total_violations);
    fprintf(f, "  \"status\": \"%s\",\n", total_violations == 0 ? "PASS" : "FAIL");

    //files array
    fprintf(f, "  \"files\": [\n");

    const FileResultNode* current = head;
    while (current != NULL) {
        //Escape backslashes in paths for valid JSON
        char escaped_filename[4096 * 2] = {0};
        const char* s = current->filename;
        char* d = escaped_filename;
        while (*s && d < escaped_filename + (sizeof(escaped_filename) - 2)) {
            if (*s == '\\') {
                *d++ = '\\';
                *d++ = '\\';
            } else {
                *d++ = *s;
            }
            s++;
        }

        fprintf(f, "    {\n");
        fprintf(f, "      \"file\": \"%s\",\n", escaped_filename);
        fprintf(f, "      \"violations\": %d\n", current->violation_count);
        fprintf(f, "    }");

        if (current->next != NULL) {
            fprintf(f, ",\n"); // Add comma if it's not the last item
        } else {
            fprintf(f, "\n"); // No comma
        }
        current = current->next;
    }

    fprintf(f, "  ]\n");
    fprintf(f, "}\n");

    fclose(f);
    printf("JSON report saved to: %s\n", output_file);


/*void export_violations_json(const char *filename, int total_violations, const char* output_file) {
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

    printf("JSON report saved to: %s\n", output_file);*/
}