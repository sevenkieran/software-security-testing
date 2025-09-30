#ifndef MEMORYLEAKDETECTOR_CONSTANTS_H
#define MEMORYLEAKDETECTOR_CONSTANTS_H

#include "argparse.h"
#include "analyze.h"

const CliArgument CLI_INPUT_ARGS[] = {
    {"help", "-h", "Shows help message"},
    {"recursive", "-r", "Recursively iterates through a directory"}
};

const Rule RULES[] = {
    {"Memory Leak", "Detects malloc/free misuses", memory_leak_rule},
    {"Deprecated Headers", "Finds deprecated header usage", deprecated_header_rule}
};

#endif //MEMORYLEAKDETECTOR_CONSTANTS_H