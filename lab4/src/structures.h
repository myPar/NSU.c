#ifndef POSTFIX_NOTATION_STRUCTURES_H
#define POSTFIX_NOTATION_STRUCTURES_H
#define value_size 20
// enum of possible token type
typedef enum {
    OPERATOR,
    IDENT,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS
} Type;
// returns string value of the enum
//char *get_string_value(Type type);

// structure implements basic block of expression component
typedef struct ExpressionToken Token;
struct ExpressionToken {
    Type type;      // Token type
    char value[value_size];     // string value: "1..9", "(", ")", "+../"
};
// constructor
Token *make_token(const char *string_value, Type type);
Token *get_token(const char *input_string, int *idx);
int get_priority(Token *token);

// linked list node structure
typedef struct ListNode Node;
struct ListNode {
    Token *token;   // token value
    Node *next;     // link on the next node
    Node *prev;     // link on the previous node
};
// constructor
Node *make_node(Token *token);
// free node memory method
void free_node(Node *node);
// calculates operation presented by 'operator' with 'arg1' and 'arg2' arguments
Node *calculate(Node *operator, Node *arg1, Node *arg2);

// linked list structure for Stack implementation
typedef struct DoubleLinkedList List;
struct DoubleLinkedList {
    Node *head;     // list head pointer
    Node *tail;     // list tail pointer
    int size;       // number of nodes field
};
// constructor
List *make_list();
// pushing new element
void push(List *list, Node *paste_node);
// cut last list element
Node *pop(List *list);
// free memory
void free_list(List *list);

#undef value_size

#endif
