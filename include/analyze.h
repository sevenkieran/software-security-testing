#ifndef ANALYZE_H
#define ANALYZE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "formatter.h"
#include "json_export.h"

typedef struct {
    char **lines;
    int line_count;
    char *filename;
} SourceFile;


typedef ViolationNode* (*RuleFunction)(const SourceFile *file);//returns number of violations found

//rule definition
typedef struct {
    char name[32];
    char description[200];
    RuleFunction function;
} Rule;

//core functions
SourceFile* load_source_file(const char *filename);
void free_source_file(SourceFile *file);
int analyze_with_rules(const char *filename);
int analyze_project(const char *path, bool is_directory, FileResultNode** results_head);

//helper functions
char* trim_line(const char *line);
bool is_comment_or_preprocessor(const char *line);

//rule functions
ViolationNode* memory_leak_rule(const SourceFile *file);
ViolationNode* deprecated_header_rule(const SourceFile *file);
ViolationNode* integer_overflow_rule(const SourceFile *file);
ViolationNode* signal_safety_rule(const SourceFile *file);
int analyze_with_rules_return_count(const char *filename);

#endif