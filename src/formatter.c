#include "formatter.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ViolationNode* create_node(int line, const char* line_code,
                                  const char* message,
                                  ViolationType type) {
    ViolationNode* newNode = (ViolationNode*)malloc(sizeof(ViolationNode));
    if (!newNode) {
        fprintf(stderr, BRED "Memory allocation failed for violation node\n" reset);
        exit(EXIT_FAILURE);
    }

    newNode->data.line_number = line;
    newNode->data.violation_type = type;

    // Duplicate strings
    newNode->data.line_code = line_code ? strdup(line_code) : NULL;
    newNode->data.message = message ? strdup(message) : NULL;


    if ((line_code && !newNode->data.line_code) ||
        (message && !newNode->data.message)) {
        fprintf(stderr, BRED "Memory allocation failed for strings\n" reset);
        free(newNode->data.line_code);
        free(newNode->data.message);
        free(newNode);
        exit(EXIT_FAILURE);
    }

    newNode->next = NULL;
    return newNode;
}

void append_violation(ViolationNode** head, int line, const char* line_code,
                     const char* message, ViolationType type) {
    ViolationNode* newNode = create_node(line, line_code, message, type);

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    ViolationNode* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
}

static const char* get_violation_type_string(ViolationType type) {
    switch (type) {
        case INTEGER_OVERFLOW:   return "Integer Overflow";
        case DEPRECATED_HEADER:  return "Deprecated Header";
        case MEMORY_LEAK:        return "Memory Leak";
        default:                 return "Unknown Violation";
    }
}

void print_violations(const ViolationNode* head, const char* rule_name) {
    if (head == NULL) {
        return; // Don't print anything if no violations
    }

    const ViolationNode* current = head;
    while (current != NULL) {
        printf(HYEL "  ⚠️  Line %d: " reset, current->data.line_number);
        if (current->data.message) {
            printf("%s\n", current->data.message);
        } else {
            printf("%s\n", get_violation_type_string(current->data.violation_type));
        }

        if (current->data.line_code && strlen(current->data.line_code) > 0) {
            printf(HCYN "      Code: %s\n" reset, current->data.line_code);
        }

        //if (current->data.suggestion) {
          //  printf(HMAG "      💡 Suggestion: %s\n" reset, current->data.suggestion);
        //}

        printf("\n");
        current = current->next;
    }
}

void free_violations(ViolationNode* head) {
    ViolationNode* current = head;
    ViolationNode* next_node;

    while (current != NULL) {
        next_node = current->next;
        free(current->data.line_code);
        free(current->data.message);
        //free(current->data.suggestion);
        free(current);
        current = next_node;
    }
}

int count_violations(const ViolationNode* head) {
    int count = 0;
    const ViolationNode* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}