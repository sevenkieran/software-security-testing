#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "argparse.h"

#include "colors.h"


void print_usage(const char *prog, const struct option *long_options)
{
    printf(WHT"Usage: %s [options] file|directory\n"CLRreset, prog);
    printf(CYN"  Options:\n"CLRreset);

    for (int i = 0; long_options[i].name != NULL; i++) {
        char short_opt = long_options[i].val;
        printf("  -%c, --%-10s\n", short_opt, long_options[i].name);
    }
    printf("\n");
}
