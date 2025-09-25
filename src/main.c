#include "analyze.h"
const char* find_test_file() {
    //possible paths
    const char* possible_paths[] = {
        "../test/leak.c",
        "test/leak.c",
        "./test/leak.c",
        "../../test/leak.c",
        "../../../test/leak.c"
    };

    int num_paths = sizeof(possible_paths);

    for (int i = 0; i < num_paths; i++) {
        FILE *test_file = fopen(possible_paths[i], "r");
        if (test_file != NULL) {
            fclose(test_file);
            return possible_paths[i];  //found path
        }
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    printf("Memory Leak Detector\n");
    printf("===================\n");

    // Use command line argument or try to find test file
    const char *test_file;
    if (argc > 1) {
        test_file = argv[1];
        printf("Using provided file: %s\n\n", test_file);
    } else {
        test_file = find_test_file();
        if (test_file == NULL) {
            printf("Error: Could not find test/leak.c in any expected location.\n");
            printf("Please run with: %s <path_to_c_file>\n", argv[0]);
            printf("Or create test/leak.c in your project directory.\n");
            return 1;
        }
        printf("Found test file: %s\n\n", test_file);
    }


    Rule rules[] = {
        {"Memory Leak", "Detects malloc/free misuses", memory_leak_rule},
        {"Deprecated Headers", "Finds deprecated header usage", deprecated_header_rule}
    };

    int ruleCount = 2; // has to be hardcoded or it returns gibberish/bad memory

    // Run analysis
    analyze_with_rules(test_file, rules, ruleCount);

    return 0;
}
