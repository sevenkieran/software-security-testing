//
// Created by ryanb on 10/30/2025.
//
#include <stdio.h>
#ifndef MEMORYLEAKDETECTOR_JSON_EXPORT_H
#define MEMORYLEAKDETECTOR_JSON_EXPORT_H
typedef struct FileResultNode {
    char* filename;
    int violation_count;
    struct FileResultNode* next;
} FileResultNode;
void export_violations_json(const char*filename, int total_violations, const FileResultNode* head, const char *output_file);


void append_file_result(FileResultNode** head, const char* filename, int violation_count);
void free_file_results(FileResultNode* head);
#endif