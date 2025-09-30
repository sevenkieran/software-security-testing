#ifndef MEMORYLEAKDETECTOR_CONSTANTS_H
#define MEMORYLEAKDETECTOR_CONSTANTS_H

#include "argparse.h"

CliArgument CLI_INPUT_ARGS[] = {
    {"help", "-h", "Shows help message"},
    {"recursive", "-r", "Recursively iterates through a directory"}
};

#endif //MEMORYLEAKDETECTOR_CONSTANTS_H