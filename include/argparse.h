#ifndef MEMORYLEAKDETECTOR_ARGPARSE_H
#define MEMORYLEAKDETECTOR_ARGPARSE_H

typedef struct
{
    const char* name;
    const char* flag;
    const char* help;
} CliArgument;

/**
 * Parses and validates arguments
 *
 * @param[in] argc
 * @param[in] argv
 * @param[in] arguments
 * @param[in] num_args
 *
 * @return True if successful, false otherwise.
 */
const char* parse_args(int argc,
                       const char* argv[],
                       int num_args,
                       const CliArgument arguments[]);

/**
 *
 * Prints out the usage message.
 *
 * @param[in] arguments
 * @param[in] num_of_args
 * @param[in] program_name
 */
void print_usage(const CliArgument arguments[],
                 unsigned int num_of_args,
                 const char* program_name);

#endif //MEMORYLEAKDETECTOR_ARGPARSE_H