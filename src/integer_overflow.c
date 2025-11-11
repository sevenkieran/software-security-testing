#include "analyze.h"
#include "formatter.h"
#include <string.h>

ViolationNode* integer_overflow_rule(const SourceFile *file) {
    ViolationNode* violations = NULL;
    
    for (int i = 0; i < file->line_count; i++) {
        char *line = file->lines[i];
        
        if (is_comment_or_preprocessor(line)) continue;

        //Check for multiplication
        if ((strstr(line, "malloc(") || strstr(line, "calloc(")) && strstr(line, "*")) {
            append_violation(&violations,
                           i + 1,
                           line,
                           "Potential integer overflow in allocation",
                           INTEGER_OVERFLOW);
        }
        //Check for array size calculations, may be over
        if (strstr(line, "malloc(") && (strstr(line, " + ") || strstr(line, "++"))) {
            append_violation(&violations,
                           i + 1,
                           line,
                           "Potential overflow in size calculation",
                           INTEGER_OVERFLOW);
        }
    }

    return violations;
}