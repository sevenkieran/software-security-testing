#include "analyze.h"

int main(int argc, char *argv[]) {
    printf("Memory Leak Detector\n");
    printf("===================\n");




    Rule rules[] = {
        {"Memory Leak", "Detects malloc/free misuses", memory_leak_rule},
        {"Deprecated Headers", "Finds deprecated header usage", deprecated_header_rule}
    };

    int ruleCount = 2; // has to be hardcoded or it returns gibberish/bad memory

    // Run analysis
    analyze_with_rules(test_file, rules, ruleCount);

    return 0;
}
