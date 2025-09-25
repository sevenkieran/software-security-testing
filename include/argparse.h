//
// Created by kseven on 9/25/25.
//

#ifndef MEMORYLEAKDETECTOR_ARGPARSE_H
#define MEMORYLEAKDETECTOR_ARGPARSE_H

typedef struct
{
    const char* name;
    const char* flag;
    const char* help;
} CliArgument;

void parseArgs(int argc, char* argv[]);

#endif //MEMORYLEAKDETECTOR_ARGPARSE_H