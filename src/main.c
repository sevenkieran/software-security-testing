#include "analyze.h"

int main(int argc, char *argv[]) {
    printf("Memory Leak Detector\n");
    printf("===================\n");

    // Use command line argument or default to test file
    const char *test_file;
    if (argc > 1) {
        test_file = argv[1];
    } else {

        test_file = "../test/leak.c";  //look for relative or any file
    }

    printf("Analyzing: %s\n\n", test_file);

    // Define rules array (no buffer overflow rule as requested)
    Rule rules[] = {
        {"Memory Leak", "Detects malloc/free misuses", memory_leak_rule},
        {"Deprecated Headers", "Finds deprecated header usage", deprecated_header_rule}

    };

    int ruleCount = 2; //has to be hardcoded or it returns gibberish/bad memory

    // Run analysis
    analyze_with_rules(test_file, rules, ruleCount);

    return 0;
}