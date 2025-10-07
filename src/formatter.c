// src/formatter.c

#include "../include/formatter.h"
#include "../include/colors.h" // Assuming colors.h is in the include directory
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static ViolationNode* create_node(int line, const char* line_code, ViolationType type) {
    ViolationNode* newNode = (ViolationNode*)malloc(sizeof(ViolationNode));
    if (!newNode) {
        fprintf(stderr, BRED "Memory allocation failed for new violation node\n" reset);
        exit(EXIT_FAILURE);
    }


    newNode->data.line_number = line;//populate Violation data
    newNode->data.violation_type = type;


    newNode->data.line_code = strdup(line_code);//allocate memory and copy next line
    if (!newNode->data.line_code) {
        fprintf(stderr, BRED "Memory allocation failed for line_code string\n" reset);
        free(newNode);
        exit(EXIT_FAILURE);
    }

    newNode->next = NULL;
    return newNode;
}

void append_violation(ViolationNode** head, int line, const char* line_code, ViolationType type) {
    ViolationNode* newNode = create_node(line, line_code, type);


    if (*head == NULL) {//if empty, new node is now head
        *head = newNode;
        return;
    }

    ViolationNode* current = *head;//continue traversing
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = newNode;
}

//replacement for current RULES array
static const char* get_violation_type_string(ViolationType type) {
    switch (type) {
        case INTEGER_OVERFLOW:
            return "Integer Overflow";
        case DEPRECATED_HEADER:
            return "Deprecated Header";
        default:
            return "Unknown Violation";
    }
}

void print_violations(const ViolationNode* head) {
    if (head == NULL) {
        printf(BGRN "No violations found.\n" reset);
        return;
    }

    printf(BHYEL "--- Analyzing ---\n" reset);
    const ViolationNode* current = head;
    int count = 1;
    while (current != NULL) {
        printf(BHWHT "\nViolation #%d\n" reset, count++);
        printf(HYEL "  Type: " reset "%s\n", get_violation_type_string(current->data.violation_type));
        printf(HYEL "  Line: " reset "%d\n", current->data.line_number);
        printf(HYEL "  Code: " reset BHCYN "`%s`\n" reset, current->data.line_code);
        current = current->next;
    }
    printf(BHYEL "\n--- End of Report ---\n" reset);
}

void free_violations(ViolationNode* head) {
    ViolationNode* current = head;
    ViolationNode* next_node;

    while (current != NULL) {
        next_node = current->next;
        free(current->data.line_code); //free duplicated string
        free(current);
        current = next_node;
    }
}