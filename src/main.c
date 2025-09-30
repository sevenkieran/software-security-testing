#include "analyze.h"
#include "argparse.h"
#include "constants.h"

int main(int argc, char *argv[]) {

    Rule rules[] = {
        {"Memory Leak", "Detects malloc/free misuses", memory_leak_rule},
        {"Deprecated Headers", "Finds deprecated header usage", deprecated_header_rule}
    };

    const int RULES_COUNT = 2; // has to be hardcoded, or it returns gibberish/bad memory
    const int NUM_OF_CLI_ARGS = sizeof(CLI_INPUT_ARGS) / sizeof(CLI_INPUT_ARGS[0]);

    if (argc == 1) {
        print_usage(CLI_INPUT_ARGS, NUM_OF_CLI_ARGS, argv[0]);
        return 1;
    }

    const char* path = parse_args(argc, argv, NUM_OF_CLI_ARGS, CLI_INPUT_ARGS);

    printf("Path argument received: %s\n", path);

    // Run analysis
    analyze_with_rules(path, rules, RULES_COUNT);

    return 0;
}
