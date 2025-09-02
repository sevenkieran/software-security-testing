#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct MemInfo {
    void *ptr;
    size_t size;
    const char *file;
    int line;
    struct MemInfo *next;
} MemInfo;

static MemInfo *alloc_list = NULL;

void add_alloc(void *ptr, size_t size, const char *file, int line) {
    MemInfo *info = malloc(sizeof(MemInfo));
    info->ptr = ptr;
    info->size = size;
    info->file = file;
    info->line = line;
    info->next = alloc_list;
    alloc_list = info;
}

void remove_alloc(void *ptr) {
    MemInfo **curr = &alloc_list;
    while (*curr) {
        if ((*curr)->ptr == ptr) {
            MemInfo *to_free = *curr;
            *curr = (*curr)->next;
            free(to_free);
            return;
        }
        curr = &(*curr)->next;
    }
    // free called on untracked pointer
    fprintf(stderr, "Warning: freeing untracked pointer %p\n", ptr);
}

void *dbg_malloc(size_t size, const char *file, int line) {
    void *ptr = malloc(size);
    add_alloc(ptr, size, file, line);
    return ptr;
}

void dbg_free(void *ptr, const char *file, int line) {
    remove_alloc(ptr);
    free(ptr);
}

void report_leaks() {
    MemInfo *curr = alloc_list;
    if (!curr) {
        printf("No memory leaks detected!\n");
        return;
    }
    printf("Memory leaks detected:\n");
    while (curr) {
        printf("  Leak: %p (size %zu) allocated at %s:%d\n",
               curr->ptr, curr->size, curr->file, curr->line);
        curr = curr->next;
    }
}

// Macros to capture file & line
#define malloc(s) dbg_malloc(s, __FILE__, __LINE__)
#define free(p) dbg_free(p, __FILE__, __LINE__)

int main() {
    // char *leak = malloc(100);   // not freed → leak
    char *ok = malloc(50);
    free(ok);

    atexit(report_leaks);  // runs at program exit
    return 0;
}
