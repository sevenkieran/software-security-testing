#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "argparse.h"


void print_usage(const CliArgument arguments[], const unsigned int num_of_args, const char* program_name)
{
    printf("Usage: %s [options] file|directory\n", program_name);
    printf("  Options:\n");

    for (unsigned int i = 0; i < num_of_args; ++i)
    {
        printf("    %s, --%s     %s\n", arguments[i].flag, arguments[i].name, arguments[i].help);
    }
}

const char* parse_args(int argc, const char* argv[], int num_args, const CliArgument arguments[]) {
    const char* path_arg = NULL;

    for (int i = 1; i < argc; i++) {
        const char* arg = argv[i];
        int recognized = 0;

        // if it looks like an option
        if (arg[0] == '-') {
            for (int j = 0; j < num_args; j++) {
                char long_form[64];
                snprintf(long_form, sizeof(long_form), "--%s", arguments[j].name);

                if (strcmp(arg, arguments[j].flag) == 0 || strcmp(arg, long_form) == 0) {
                    recognized = 1;
                    if (strcmp(arguments[j].name, "help") == 0) {
                        print_usage(arguments, num_args, argv[0]);
                        exit(0);
                    }
                    // could set a flag somewhere if needed
                    break;
                }
            }

            if (!recognized) {
                fprintf(stderr, "Error: unrecognized option '%s'\n\n", arg);
                print_usage(arguments, num_args, argv[0]);
                exit(1);
            }
        } else {
            // non-flag arg = path
            if (path_arg) {
                fprintf(stderr, "Error: multiple paths specified: '%s' and '%s'\n\n", path_arg, arg);
                print_usage(arguments, num_args, argv[0]);
                exit(1);
            }
            path_arg = arg;
        }
    }

    if (!path_arg) {
        fprintf(stderr, "Error: missing file/directory path\n\n");
        print_usage(arguments, num_args, argv[0]);
        exit(1);
    }

    return path_arg;
}