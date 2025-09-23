#include <stdio.h>
#include <stdlib.h>

typedef struct {
    const char* message;
    size_t line;
} Finding;

void analyze_deprecated_headers(const char* code, Finding* findings, size_t* finding_count, size_t max_findings);
void analyze_leaks(const char* code, Finding* findings, size_t* finding_count, size_t max_findings);

FILE* open_test_file(const char* filename) {
    char path[512];
    snprintf(path, sizeof(path), "test/%s", filename);//open any file in test folder

    FILE* f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "Error: Could not open test file at '%s'\n", path);
    }
    return f;
}

int main(int argc, char* argv[]) {
    const char* test_filename = "leak.c";  // default test file in test folder

    FILE* f = open_test_file(test_filename);
    if (!f) {
        return 1;
    }

    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);//start scanning from beginning of file again

    char* contents = malloc(len + 1);
    fread(contents, 1, len, f);
    contents[len] = '\0';
    fclose(f);

    //Deprecated header
    Finding findings_headers[50]; //change
    size_t count_headers = 0;
    analyze_deprecated_headers(contents, findings_headers, &count_headers, 50);

    printf("=== Deprecated Header Analysis ===\n");
    if (count_headers == 0) {
        printf("No deprecated headers found.\n");
    } else {
        for (size_t i = 0; i < count_headers; i++) {
            printf("Line %zu: Deprecated header found: %s\n",
                   findings_headers[i].line, findings_headers[i].message);
        }
    }

    //Leak detection
    Finding findings_leaks[50];
    size_t count_leaks = 0;
    analyze_leaks(contents, findings_leaks, &count_leaks, 50);

    printf("\n=== Leak Analysis ===\n");
    if (count_leaks == 0) {
        printf("No potential leaks found.\n");
    } else {
        for (size_t i = 0; i < count_leaks; i++) {
            printf("Line %zu: %s\n",
                   findings_leaks[i].line, findings_leaks[i].message);
        }
    }

    free(contents);
    return 0;
}
