#include "colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include "analyze.h"
#include "constants.h"
#include "utils.h"


#define MAX_LINE_LENGTH 2048
#define INITIAL_LINES 100

SourceFile* load_source_file(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "%sCannot open file '%s'\n%s", BRED, filename, CLRreset);
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

    //read file line by line
    while (fgets(buffer, sizeof(buffer), file))
    {
        //remove newline
        buffer[strcspn(buffer, "\n\r")] = '\0';


        if (src->line_count >= capacity)
        {
            capacity *= 2;
            char** new_lines = realloc(src->lines, capacity * sizeof(char*));
            if (!new_lines)
            {
                printf("%sMemory allocation error\n%s", BRED, CLRreset);
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
    printf("%sLoaded %d lines from %s\n%s", BCYN, src->line_count, filename, CLRreset);
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

int analyze_project(const char* path, const bool is_directory)
{
    struct stat st;
    if (stat(path, &st) != 0) {
        perror("stat");
        return 0;
    }
    int grand_total_violations = 0;

    if (!is_directory) {
        // Expecting a single file
        if (S_ISREG(st.st_mode)) {
            grand_total_violations += analyze_with_rules(path);
        } else {
            fprintf(stderr, "%sError: '%s' is not a regular file\n%s", BRED, path, CLRreset);
        }
        return grand_total_violations;
    }


    if (!S_ISDIR(st.st_mode)) {
        fprintf(stderr, "%sError: '%s' is not a directory (with -r)\n%s", BRED, path, CLRreset);
        return 0;
    }

    DIR *dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return 0;
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
            grand_total_violations += analyze_project(fullpath, 1);
        } else if (S_ISREG(st.st_mode) && is_c_or_h_file(entry->d_name)) {
            grand_total_violations += analyze_with_rules(fullpath);
        }
    }

    closedir(dir);
    return grand_total_violations;
}

int analyze_with_rules(const char* filename)
{
    SourceFile* source = load_source_file(filename);
    if (!source) return 0;

    int total_violations = 0;
    printf("%s\nAnalyzing: %s\n%s", BCYN, filename, CLRreset);
    printf("%sTotal rules to check: %d\n%s", BCYN, RULE_COUNT, CLRreset);

    // Run each rule
    for (int i = 0; i < RULE_COUNT; i++)
    {
        printf("%s\n--- Checking Rule: %s ---\n%s", BWHT, RULES[i].name, CLRreset);
        printf("%sDescription: %s\n%s", BWHT, RULES[i].description, CLRreset);

        ViolationNode* violations_list = RULES[i].function(source);

        int violation_count = count_violations(violations_list);

        if (violation_count > 0)
        {
            printf("%sFound %d violation(s):\n%s", BYEL, violation_count, CLRreset);
            print_violations(violations_list, RULES[i].name);
            total_violations += violation_count;
        }
        else
        {
            printf("%sNo violations found\n%s", BGRN, CLRreset);
        }

        free_violations(violations_list);
    }

    printf("%s\n--- Analysis Complete for %s ---\n%s", BWHT, filename, CLRreset);
    printf("%sTotal violations found in this file: %d\n\n%s", BWHT, total_violations, CLRreset);

    free_source_file(source);
    return total_violations;
}

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

