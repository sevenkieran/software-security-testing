//
// Created by ryanb on 10/2/2025.
//

#include "../include/formatter.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "colors.h"

ViolationNode* createNode(int line, const char* message) {
    ViolationNode* newNode = (ViolationNode*)malloc(sizeof(ViolationNode));
    if (!newNode) {
        printf(BRED"Memory allocation failed\n"reset);
        exit(1);
    }

    newNode->violation = malloc(sizeof(Violation));
    if (!newNode->violation) {
        printf(BRED"Memory allocation failed\n"reset);
        exit(1);
    }

    newNode->violation->line_number = line;
    newNode->violation->line_code = strdup(message); // safer than strncpy
    newNode->next = NULL;

    return newNode;
}

void append_violation(ViolationNode** head, int line, const char* message) {
    ViolationNode* newNode = createNode(line, message);

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    ViolationNode* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}
