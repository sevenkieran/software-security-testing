#include <getopt.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "analyze.h"
#include "argparse.h"
#include "colors.h"
#include "formatter.h"
#include "../include/json_export.h"

int main(int argc, char *argv[]) {
    bool recursive_enabled = false;
    ViolationNode* violations_list = NULL;
    char *json_output = NULL;

    static struct option long_options[] = {
        {"help",      no_argument, 0, 'h'},
        {"recursive", no_argument, 0, 'r'},
        {"json", required_argument, 0, 'j'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "hrj", long_options, NULL)) != -1) {
        switch (opt) {
        case 'h':
            print_usage(argv[0], long_options);
            return 0;
        case 'r':
            recursive_enabled = 1;
            break;
        case 'j':
            json_output = optarg;
            break;
        default:
            print_usage(argv[0], long_options);
            return 1;
        }
    }
    if (optind >= argc) {
        print_usage(argv[0], long_options);
        return 1;
    }

    const char *path_arg = argv[optind];

    struct stat st;
    if (stat(path_arg, &st) != 0) {
        perror("stat");
        return 1;
    }

    if (recursive_enabled && !S_ISDIR(st.st_mode)) {
        fprintf(stderr, BRED"Error: '%s' is not a directory (required with -r)\n"CLRreset, path_arg);
        return 1;
    }
    if (!recursive_enabled && !S_ISREG(st.st_mode)) {
        fprintf(stderr, BRED"Error: '%s' is not a regular file\n"CLRreset, path_arg);
        return 1;
    }

    int total_violations = analyze_project(path_arg, recursive_enabled);

    printf("\nAnalysis Complete.\nTotal Violations: %d\n", total_violations);

    if (json_output) {
        export_violations_json(path_arg, total_violations, json_output);
    }



    return (total_violations > 0) ? 1 : 0;
}

