//
// Created by kseven on 9/23/25.
//

#include <string.h>
#include <stdio.h>

//void analyze(const char *fileName) {
    //rules *ruleArr = malloc(sizeof(rules));

        //leakDetect.c
        //deprecatedHeader.c
    }
}

#include <stdio.h>
#include <string.h>

// Example stub: each rule.c will provide something like this
extern int memory_leak_rule(const char *filename);
extern int deprecated_header_rule(const char *filename);

void analyze(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: analyze --old old.json --new new.json rule1.c [rule2.c ...]\n");
        return;
    }

    const char *oldFile = argv[2];
    const char *newFile = argv[3];

    printf("Comparing %s vs %s\n", oldFile, newFile);

    // Rules start from argv[4] onwards
    for (int i = 4; i < argc; i++) {
        const char *ruleFile = argv[i];
        printf("Applying rule: %s\n", ruleFile);


        if (strstr(ruleFile, analyze()) != NULL) {
            memory_leak_rule(newFile);
            argv[i++];
        } else {
            printf("Unknown rule: %s\n", ruleFile);
        }
    }
}
