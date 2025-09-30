#ifndef MEMORYLEAKDETECTOR_CONSTANTS_H
#define MEMORYLEAKDETECTOR_CONSTANTS_H

#include "argparse.h"
#include "analyze.h"

/// The different rules checks being performed
const Rule RULES[] = {
    {"Memory Leak", "Detects malloc/free misuses", memory_leak_rule},
    {"Deprecated Headers", "Finds deprecated header usage", deprecated_header_rule}
};

#endif //MEMORYLEAKDETECTOR_CONSTANTS_H