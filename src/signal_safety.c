#include "analyze.h"
#include "formatter.h"
#include <string.h>
#include <ctype.h>
#define MAX_HANDLERS 32
static const char* UNSAFE_FUNCS[] = {
    "malloc", "free", "calloc", "realloc",
    "printf", "fprintf", "sprintf", "snprintf",
    "fopen", "fclose", "system", "popen", "exit",
    NULL
};

typedef struct {
char name[64];
    int def_line;

} Handler;

static void get_handler_name(const char* line, char* out) {
    const char *p = strchr(line, ',');
    if (!p) return;
    for (p++; *p && (isspace(*p) || *p == '&'); p++);
    int i = 0;
    while (*p && (isalnum(*p) || *p == '_') && i < 63)
        out[i++] = *p++;
    out[i] = '\0';
}

//checks that line defines a function named handler()
static bool is_handler_def(const char* line, const char* name) {
    const char* pos = strstr(line, name);
    if (!pos) return false;
    if (pos > line && isalnum(pos[-1])) return false;
    pos += strlen(name);
    while (*pos && isspace(*pos)) pos++;
    return *pos == '(';
}

ViolationNode* signal_safety_rule(const SourceFile *file) {
    ViolationNode* violations = NULL;
    Handler handlers[MAX_HANDLERS];
    int handler_count = 0;

    //Find handler names
    for (int i = 0; i < file->line_count; i++) {
        char *line = file->lines[i];
        if (is_comment_or_preprocessor(line)) continue;
        if (strstr(line, "signal(")) {
            char h[64] = {0};
            get_handler_name(line, h);
            if (!*h || !strcmp(h,"SIG_DFL") || !strcmp(h,"SIG_IGN")) continue;

            bool exists = false;
            for (int j = 0; j < handler_count; j++)
                if (!strcmp(handlers[j].name, h)) exists = true;

            if (!exists && handler_count < MAX_HANDLERS)
                strcpy(handlers[handler_count++].name, h);
        }
    }

    int braces = 0, current = -1;
    for (int i = 0; i < file->line_count; i++) {
        char *line = file->lines[i];
        if (is_comment_or_preprocessor(line)) continue;

        if (current == -1) {
            for (int j = 0; j < handler_count; j++) {
                if (is_handler_def(line, handlers[j].name)) {
                    current = j;
                    handlers[j].def_line = i + 1;
                    if (strchr(line, '{')) braces++;
                    break;
                }
            }
        } else {
            for (char *c = line; *c; c++) {
                if (*c == '{') braces++;
                else if (*c == '}') braces--;
            }

            if (braces > 0) {
                for (int j = 0; UNSAFE_FUNCS[j]; j++) {
                    char pattern[72];
                    snprintf(pattern, sizeof(pattern), "%s(", UNSAFE_FUNCS[j]);
                    if (strstr(line, pattern)) {
                        char msg[256];
                        snprintf(msg, sizeof(msg),
                            "Unsafe call '%s' inside signal handler '%s' (defined on line %d).",
                            UNSAFE_FUNCS[j], handlers[current].name, handlers[current].def_line);
                        append_violation(&violations, i + 1, line, msg, SIGNAL_SAFETY);
                    }
                }
            }
            if (braces <= 0) current = -1;
        }
    }

    return violations;
}