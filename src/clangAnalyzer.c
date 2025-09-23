//
// Created by ryanb on 9/18/2025.
//
// clangAnalyzer.c
#include <stdlib.h>
#include <string.h>
#include <clang-c/Index.h>
#include <stdio.h>

typedef struct {
    const char* message;
    size_t line;
} Finding;

typedef struct {
    Finding* findings;
    size_t* count;
    size_t max;
} AnalyzerContext;

static enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    AnalyzerContext* ctx = (AnalyzerContext*) client_data;

    if (clang_getCursorKind(cursor) == CXCursor_CallExpr) {
        CXString spelling = clang_getCursorSpelling(cursor);
        const char* funcName = clang_getCString(spelling);

        if (funcName) {
            if ((strcmp(funcName, "gets") == 0) || (strcmp(funcName, "strcpy") == 0)) {
                if (*(ctx->count) < ctx->max) {
                    CXSourceLocation loc = clang_getCursorLocation(cursor);
                    unsigned line = 0;
                    clang_getSpellingLocation(loc, NULL, &line, NULL, NULL);

                    ctx->findings[*(ctx->count)].message = strdup(funcName);
                    ctx->findings[*(ctx->count)].line = line;
                    (*(ctx->count))++;
                }
            }
        }
        clang_disposeString(spelling);
    }

    return CXChildVisit_Recurse;
}

void analyze_with_clang(const char* filename, Finding* findings, size_t* count, size_t max) {
    CXIndex idx = clang_createIndex(0, 0);
    CXTranslationUnit tu = clang_parseTranslationUnit(
        idx, filename, NULL, 0, NULL, 0, CXTranslationUnit_None);

    if (tu == NULL) {
        fprintf(stderr, "Error: Could not parse %s with libclang\n", filename);
        return;
    }

    CXCursor root = clang_getTranslationUnitCursor(tu);
    AnalyzerContext ctx = { findings, count, max };
    clang_visitChildren(root, visitor, &ctx);

    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(idx);
}
