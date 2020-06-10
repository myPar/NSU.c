#include "structures.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
// gets string value of the 'Type' enum
char *get_string_value(Type type) {
    switch (type) {
        case OPERATOR: return "OPERATOR";
        case IDENT: return "IDENT";
        case OPEN_PARENTHESIS: return "OPEN_PARENTHESIS";
        case CLOSE_PARENTHESIS: return "CLOSE_PARENTHESIS";
        default:
            printf("unrecognized enum, should be 'Type' enum\n");
            exit(1);
    }
}

// structures constructors implementation
Token *make_token(const char *value, Type type) {
    Token *token = (Token*) malloc(sizeof(Token));
    token->type = type;
    strcpy(token->value, value);    // copying string data into the field
    return token;
}

Node *make_node(Token *token) {
    Node *node = (Node*) malloc(sizeof(Node));
    node->token = token;
    node->next = NULL;
    node->prev = NULL;
    return node;
}
void free_node(Node *node) {
    free(node->token);
    free(node);
}

List *make_list() {
    List *list = (List*) malloc(sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

// list methods
void push(List *list, Node *node) {
    if (list->size == 0) {          // push element in empty list
        list->head = node;
        list->tail = node;
    }
    else {
        list->tail->next = node;    // insert element at the end of the list
        node->prev = list->tail;    // initialize 'prev' field pointer
        list->tail = node;          // move tail pointer
    }
    list->size++;                   // increase list size
}
Node *pop(List *list) {
    Node *tail = list->tail;        // member the list tail pointer

    if (list->size <= 0) {
        printf("can't pop element, the list is empty\n");
        exit(1);
    }
    if (list->size == 1) {
        list->tail = NULL;
        list->head = NULL;
    }
    else {
        list->tail = list->tail->prev;
        tail->prev = NULL;
        list->tail->next = NULL;
    }
    list->size--;
    return tail;                    // return removed element
}
void free_list(List *list) {
    Node *cur_node = list->tail->prev;
    int cur_size = list->size;

    for (int i = 0; i < cur_size; i++) {
        free(list->tail->token);
        free(list->tail);
        list->tail = cur_node;

        if (cur_node != NULL) {
            cur_node = cur_node->prev;
        }
    }
    free(list);
}

// other methods
bool is_digit(char ch) {
    return (ch - '0' >= 0) && ((ch - '0' <= 9)) ? true : false;
}

int get_ident_size(const char *expression_string, int *idx) {
    int ident_size = 0;

    if (expression_string[*idx] == '0' && is_digit(expression_string[*idx + 1])) {
        return ident_size;
    }
    while (is_digit(expression_string[*idx])) {
        (*idx)++;
        ident_size++;
    }
    return ident_size;
}
// returns token in case of success and NULL otherwise
Token *get_token(const char *expression_string, int *idx) {
    char value_str[20] = {0};
    value_str[0] = expression_string[*idx];
    Type token_type;
    int ident_size;

    switch (value_str[0]) {
        case '+':
        case '-':
        case '*':
        case '/':
            token_type = OPERATOR;
            (*idx)++;
            break;
        case '(':
            token_type = OPEN_PARENTHESIS;
            (*idx)++;
            break;
        case ')':
            token_type = CLOSE_PARENTHESIS;
            (*idx)++;
            break;
        default:
            // all other cases have been checked, so this is an IDENT token
            ident_size = get_ident_size(expression_string, idx);

            if (!ident_size) {
                // if ident size is equal zero - syntax error
                return  NULL;
            }
            for (int i = 0; i < ident_size; i++) {
                value_str[i] = expression_string[*idx - ident_size + i];
            }
            token_type = IDENT;
    }
    return make_token(value_str, token_type);
}

// input token always has 'operator' type
int get_priority(Token *token) {
    switch(token->value[0]) {
        case '*':
        case '/':
            return 2;
        case '+':
        case '-':
            return 1;
        default:
            printf("unrecognized operator\n");
            exit(1);
    }
}
// itoa() doesn't recognized by compiler on gitlab
char *int_to_string(int number, char *result) {
    int idx = 0;
    int delta = 0;

    if (number < 0) {
        result[idx] = '-';
        idx++;
        number = -number;
        delta = 1;
    }
    while (number > 0) {
        result[idx] = (char)('0' + number % 10);
        number /= 10;
        idx++;
    }
    result[idx] = '\0';
    // reverse string
    for (int i = 0; i < idx / 2; i++) {
        char copy = result[i + delta];
        result[i + delta] = result[idx - 1 - i];
        result[idx - 1 - i] = copy;
    }
    return result;
}
// 'operator' always have OPERATOR token type
Node *calculate(Node *operator, Node *arg1, Node *arg2) {
    int result = 0;
    int arg1_int = atoi(arg1->token->value);
    int arg2_int = atoi(arg2->token->value);

    switch((operator->token->value)[0]) {
        case '+':
            result = arg1_int + arg2_int;
            break;
        case '-':
            result = arg1_int - arg2_int;
            break;
        case '*':
            result = arg1_int * arg2_int;
            break;
        case '/':
            if (arg2_int == 0) {
                return NULL;
            }
            result  = arg1_int / arg2_int;
    }
    char result_str[20] = {0};
    int_to_string(result, result_str);
    // make node token
    Token *node_token = make_token(result_str, IDENT);

    return make_node(node_token);
}
