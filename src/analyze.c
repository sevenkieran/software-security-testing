#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "analyze.h"
#include "constants.h"
#include "utils.h"
#include "colors.h"

#define MAX_LINE_LENGTH 2048
#define INITIAL_LINES 100

SourceFile* load_source_file(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        printf(BRED"Cannot open file '%s'\n"CLRreset, filename);
        return NULL;
    } //returns pointer that holds filename, code and linecount

    SourceFile* src = malloc(sizeof(SourceFile));
    if (!src){
        fclose(file);
        return NULL;
    } //allocate space that holds filename and lines array

    src->filename = malloc(strlen(filename) + 1);
    strcpy(src->filename, filename); //store file name
    src->line_count = 0;

    //allocate space, each 'lines' holds a line of code, can expand with realloc
    int capacity = INITIAL_LINES;
    src->lines = malloc(capacity * sizeof(char*));

    char buffer[MAX_LINE_LENGTH];

    // Read file line by line
    while (fgets(buffer, sizeof(buffer), file))
    {
        // Remove newline
        buffer[strcspn(buffer, "\n\r")] = '\0';

        // Expand array if needed
        if (src->line_count >= capacity)
        {
            capacity *= 2;
            char** new_lines = realloc(src->lines, capacity * sizeof(char*));
            if (!new_lines)
            {
                printf(BRED"Memory allocation error\n"CLRreset);
                break;
            }
            src->lines = new_lines;
        }

        // Store the line
        src->lines[src->line_count] = malloc(strlen(buffer) + 1);
        strcpy(src->lines[src->line_count], buffer);
        src->line_count++; //allocate enough to store line including null terminator, then copy to new pointer
    }

    fclose(file);
    printf(BCYN"Loaded %d lines from %s\n"CLRreset,src->line_count, filename);
    return src;
}

void free_source_file(SourceFile* file)
{
    if (file)
    {
        for (int i = 0; i < file->line_count; i++)
        {
            free(file->lines[i]);
        }
        free(file->lines);
        free(file->filename);
        free(file); //free everything
    }
}

void analyze_project(const char* path, const bool is_directory)
{
    struct stat st;
    if (stat(path, &st) != 0) {
        perror("stat");
        return;
    }

    if (!is_directory) {
        // Expecting a single file
        if (S_ISREG(st.st_mode)) {
            analyze_with_rules(path);
        } else {
            fprintf(stderr, BRED"Error: '%s' is not a regular file\n"CLRreset, path);
        }
        return;
    }

    // Recursive mode: must be a directory
    if (!S_ISDIR(st.st_mode)) {
        fprintf(stderr, BRED"Error: '%s' is not a directory (with -r)\n"CLRreset, path);
        return;
    }

    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        // Build full path
        char fullpath[4096];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        if (stat(fullpath, &st) != 0) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            // Recurse into subdirectory
            analyze_project(fullpath, 1);
        } else if (S_ISREG(st.st_mode) && is_c_or_h_file(entry->d_name)) {
            analyze_with_rules(fullpath);
        }
    }

    closedir(dir);
}

void analyze_with_rules(const char* filename)
{
    SourceFile* source = load_source_file(filename);
    if (!source) return;

    int total_violations = 0;
    printf(BCYN"\nAnalyzing: %s\n"CLRreset, filename);
    printf(BCYN"Total rules to check: %d\n"CLRreset, RULE_COUNT);

    // Run each rule
    for (int i = 0; i < RULE_COUNT; i++)
    {
        printf(BWHT"\n--- Checking Rule: %s ---\n"CLRreset, RULES[i].name);
        printf(BWHT"Description: %s\n"CLRreset, RULES[i].description);

        ViolationNode* violations_list = RULES[i].function(source);

        int violation_count = count_violations(violations_list);

        if (violation_count > 0)
        {
            printf(BYEL"Found %d violation(s):\n"CLRreset, violation_count);
            print_violations(violations_list, RULES[i].name);
            total_violations += violation_count;
        }
        else
        {
            printf(BGRN"No violations found\n"CLRreset);
        }

        free_violations(violations_list);
    }

    printf(BWHT"\n--- Analysis Complete for %s ---\n"CLRreset, filename);
    printf(BWHT"Total violations found in this file: %d\n\n"CLRreset, total_violations);

    free_source_file(source);
}

// Helper functions for rules
char* trim_line(const char* line)
{
    while (*line == ' ' || *line == '\t') line++;
    return (char*)line;
}

bool is_comment_or_preprocessor(const char* line)
{
    char* trimmed = trim_line(line);
    return (trimmed[0] == '/' && trimmed[1] == '/') || trimmed[0] == '#';
}
