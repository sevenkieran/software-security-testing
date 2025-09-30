#ifndef MEMORYLEAKDETECTOR_ARGPARSE_H
#define MEMORYLEAKDETECTOR_ARGPARSE_H

#include <getopt.h>

/**
 *
 * @param prog
 * @param long_options
 */
void print_usage(const char *prog,
                 const struct option *long_options);

#endif //MEMORYLEAKDETECTOR_ARGPARSE_H