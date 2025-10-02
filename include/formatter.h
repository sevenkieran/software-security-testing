//
// Created by ryanb on 10/2/2025.
//

#ifndef MEMORYLEAKDETECTOR_FORMATTER_H
#define MEMORYLEAKDETECTOR_FORMATTER_H

#endif //MEMORYLEAKDETECTOR_FORMATTER_H

typedef enum {
    INTEGER_OVERFLOW,

} ViolationType;

typedef struct {
    int line_number;
    char *line_code;
    ViolationType violation_type;
} Violation;

typedef struct {
    Violation *violation;
    struct ViolationNode* next;

} ViolationNode;

ViolationNode* createNode(int line, const char* message) {
    ViolationNode* newNode = (ViolationNode*)malloc(sizeof(ViolationNode));
    if (!newNode) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    newNode->data.line = line;
    strncpy(newNode->data.message, message, sizeof(newNode->data.message) - 1);
    newNode->data.message[sizeof(newNode->data.message) - 1] = '\0'; // ensure null-termination
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
