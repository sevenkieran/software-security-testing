#include "analyze.h"

#define MAX_LINE_LENGTH 2048
#define INITIAL_LINES 100

SourceFile* load_source_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file '%s'\n", filename);
        return NULL;
    }//returns pointer that holds filename, code and linecount

    SourceFile *src = malloc(sizeof(SourceFile));
    if (!src) {
        fclose(file);
        return NULL;
    }//allocate space that holds filename and lines array

    src->filename = malloc(strlen(filename) + 1);
    strcpy(src->filename, filename);//store file name
    src->line_count = 0;

    //allocate space, each 'lines' holds a line of code, can expand with realloc
    int capacity = INITIAL_LINES;
    src->lines = malloc(capacity * sizeof(char*));

    char buffer[MAX_LINE_LENGTH];

    // Read file line by line
    while (fgets(buffer, sizeof(buffer), file)) {
        // Remove newline
        buffer[strcspn(buffer, "\n\r")] = '\0';

        // Expand array if needed
        if (src->line_count >= capacity) {
            capacity *= 2;
            char **new_lines = realloc(src->lines, capacity * sizeof(char*));
            if (!new_lines) {
                printf("Memory allocation error\n");
                break;
            }
            src->lines = new_lines;
        }

        // Store the line
        src->lines[src->line_count] = malloc(strlen(buffer) + 1);
        strcpy(src->lines[src->line_count], buffer);
        src->line_count++;//allocate enough to store line including null terminator, then copy to new pointer
    }

    fclose(file);
    printf("Loaded %d lines from %s\n", src->line_count, filename);
    return src;
}

void free_source_file(SourceFile *file) {
    if (file) {
        for (int i = 0; i < file->line_count; i++) {
            free(file->lines[i]);
        }
        free(file->lines);
        free(file->filename);
        free(file);//free everything
    }
}

void analyze_with_rules(const char *filename, Rule *rules, int rule_count) {
    SourceFile *source = load_source_file(filename);
    if (!source) return;

    int total_violations = 0;

    // Run each rule
    for (int i = 0; i < rule_count; i++) {
        printf("\nChecking %s ---\n", rules[i].name);
        printf("Description: %s\n", rules[i].description);

        int violations = rules[i].function(source);

        if (violations > 0) {
            printf("Found %d violation(s)\n", violations);
            total_violations += violations;
        } else {
            printf("No violations found\n");
        }
    }

    printf("\nAnalysis Complete\n");
    printf("Total violations: %d\n", total_violations);

    free_source_file(source);
}

// Helper functions for rules
char* trim_line(const char *line) {
    while (*line == ' ' || *line == '\t') line++;
    return (char*)line;
}

bool is_comment_or_preprocessor(const char *line) {
    char *trimmed = trim_line(line);
    return (trimmed[0] == '/' && trimmed[1] == '/') || trimmed[0] == '#';
}