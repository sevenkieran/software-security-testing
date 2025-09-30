#include "analyze.h"
#include "argparse.h"
#include "constants.h"

int main(int argc, char *argv[]) {

    const int RULES_COUNT = sizeof(RULES) / sizeof(RULES[0]);
    const int NUM_OF_CLI_ARGS = sizeof(CLI_INPUT_ARGS) / sizeof(CLI_INPUT_ARGS[0]);

    if (argc == 1) {
        print_usage(CLI_INPUT_ARGS, NUM_OF_CLI_ARGS, argv[0]);
        return 1;
    }

    const char* path = parse_args(argc, argv, NUM_OF_CLI_ARGS, CLI_INPUT_ARGS);

    printf("Path argument received: %s\n", path);

    // Run analysis
    analyze_with_rules(path, RULES, RULES_COUNT);

    return 0;
}
