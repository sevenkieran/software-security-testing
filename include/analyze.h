#ifndef ANALYZE_H
#define ANALYZE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char **lines;
    int line_count;
    char *filename;
} SourceFile;


typedef int (*RuleFunction)(const SourceFile *file);//returns number of violations found

// Rule definition
typedef struct {
    char name[32];
    char description[512];
    RuleFunction function;
} Rule;

// Core functions (from src/analyze.c)
SourceFile* load_source_file(const char *filename);
void free_source_file(SourceFile *file);
void analyze_with_rules(const char *filename, const Rule *rules, int rule_count);

// Helper function for rules
char* trim_line(const char *line);
bool is_comment_or_preprocessor(const char *line);

// Rule functions (from individual rule files in src/)
int memory_leak_rule(const SourceFile *file);
int deprecated_header_rule(const SourceFile *file);
int integer_overflow_rule(const SourceFile *file);

#endif