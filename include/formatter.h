// include/formatter.h

#ifndef MEMORYLEAKDETECTOR_FORMATTER_H
#define MEMORYLEAKDETECTOR_FORMATTER_H




typedef enum {
    INTEGER_OVERFLOW,
    DEPRECATED_HEADER,

} ViolationType;

typedef struct {
    int line_number;
    char *line_code;
    ViolationType violation_type;
} Violation;


typedef struct ViolationNode {
    Violation data;
    struct ViolationNode* next;
} ViolationNode;



//Appends a new violation to the end of the linked list
//head- pointer to the head pointer of the list
//line- line number where the violation occurred

void append_violation(ViolationNode** head, int line, const char* line_code, ViolationType type);


void print_violations(const ViolationNode* head);


void free_violations(ViolationNode* head);

#endif //MEMORYLEAKDETECTOR_FORMATTER_H