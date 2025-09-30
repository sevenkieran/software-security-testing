#include <getopt.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "analyze.h"
#include "argparse.h"
#include "constants.h"

int main(int argc, char *argv[]) {
    bool recursive_enabled = 0;

    static struct option long_options[] = {
        {"help",      no_argument, 0, 'h'},
        {"recursive", no_argument, 0, 'r'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "hr", long_options, NULL)) != -1) {
        switch (opt) {
        case 'h':
            print_usage(argv[0], long_options);
            return 0;
        case 'r':
            recursive_enabled = 1;
            break;
        default:
            print_usage(argv[0], long_options);
            return 1;
        }
    }
    if (optind >= argc) {
        print_usage(argv[0], long_options);
        return 1;   // <-- make sure we stop here
    }

    const char *path_arg = argv[optind];

    struct stat st;
    if (stat(path_arg, &st) != 0) {
        perror("stat");
        return 1;
    }

    if (recursive_enabled && !S_ISDIR(st.st_mode)) {
        fprintf(stderr, "Error: '%s' is not a directory (required with -r)\n", path_arg);
        return 1;
    } else if (!recursive_enabled && !S_ISREG(st.st_mode)) {
        fprintf(stderr, "Error: '%s' is not a regular file\n", path_arg);
        return 1;
    }

    // Run analysis
    analyze_with_rules(path_arg, RULES, sizeof(RULES) / sizeof(RULES[0]));

    return 0;
}
