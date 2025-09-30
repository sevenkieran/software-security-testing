//
// Created by ryanb on 9/30/2025.
//
#include "constants.h"

const Rule RULES[] =
{
    {"Memory Leak", "Detects malloc/free misuses", memory_leak_rule},
    {"Deprecated Headers", "Finds deprecated header usage", deprecated_header_rule},
    {"Integer Overflow", "Checks for multiplication in malloc arguments", integer_overflow_rule}
    };

const int RULE_COUNT = sizeof(RULES) / sizeof(RULES[0]);