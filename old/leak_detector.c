define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

typedef struct MemInfo {
    void *ptr;
    size_t size;
    struct MemInfo *next;
} MemInfo;

static MemInfo *alloc_list = NULL;
static void *(*real_malloc)(size_t) = NULL;
static void (*real_free)(void *) = NULL;

void add_alloc(void *ptr, const size_t size) {
    MemInfo *info = real_malloc(sizeof(MemInfo));
    info->ptr = ptr;
    info->size = size;
    info->next = alloc_list;
    alloc_list = info;
}

void remove_alloc(const void *ptr) {
    MemInfo **curr = &alloc_list;
    while (*curr) {
        if ((*curr)->ptr == ptr) {
            MemInfo *to_free = *curr;
            *curr = (*curr)->next;
            real_free(to_free);
            return;
        }
        curr = &(*curr)->next;
    }
}

void *malloc(const size_t size) {
    if (!real_malloc)
        real_malloc = dlsym(RTLD_NEXT, "malloc");
    void *ptr = real_malloc(size);
    add_alloc(ptr, size);
    return ptr;
}

void free(void *ptr) {
    if (!real_free)
        real_free = dlsym(RTLD_NEXT, "free");
    remove_alloc(ptr);
    real_free(ptr);
}

__attribute__((destructor)) void report_leaks() {
    MemInfo *curr = alloc_list;
    if (!curr) {
        fprintf(stderr, "No memory leaks detected!\n");
        return;
    }
    fprintf(stderr, "Memory leaks detected:\n");
    while (curr) {
        fprintf(stderr, "  Leak: %p (size %zu)\n", curr->ptr, curr->size);
        curr = curr->next;
    }
}