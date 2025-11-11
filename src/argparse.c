#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "argparse.h"

#include "colors.h"


void print_usage(const char *prog, const struct option *long_options)
{
    printf(WHT"Usage: %s [options] file|directory\n"CLRreset, prog);
    printf(CYN"  Options:\n"CLRreset);
    printf("  -h, --help       Show this help message\n");
    printf("  -r, --recursive  Analyze directory recursively\n");
    printf("  -j, --json FILE  Export results to a JSON file\n");
    printf("\nExamples:\n");
    printf("  %s file.c                       # Analyze single file\n", prog);
    printf("  %s -r src/                      # Analyze directory\n", prog);
    printf("  %s -r --json report.json src/ # Analyze directory and export to JSON\n", prog);

}

