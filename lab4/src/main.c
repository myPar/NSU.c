#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "structures.h"
#include "expression_correctness.h"
#include "postfix_notation.h"

#define EXIT()                                          \
    for (int i = 0; i < (int) expression_len; i++) {    \
        if (token_list[i] != NULL) {                    \
            free(token_list[i]);                        \
        }                                               \
    }                                                   \
    free(token_list);                                   \
    exit(0);

int main(int argc, char *argv[]) {
    FILE *input = set_input(argc, argv);
    char input_string[1024] = {0};

    size_t expression_len;
    int cur_idx = 0;

    if (!(fgets(input_string, 1024, input))) {
        printf("can't read a string");
        exit(1);
    }
    expression_len = strlen(input_string);
    // cut '\n' from the end of the string
    if (input_string[expression_len - 1] == '\n') {
        input_string[expression_len - 1] = '\0';
        expression_len--;
        if (expression_len == 0) {
            // input string is empty
            printf("syntax error");
            exit(0);
        }
    }
    Token **token_list = (Token**) malloc(sizeof(Token*) * expression_len);
    // first initialization
    for (int i = 0; i < (int) expression_len; i++) {
        token_list[i] = NULL;
    }
    int list_idx = 0;

    while (cur_idx < (int) expression_len) {
        if ((token_list[list_idx] = get_token(input_string, &cur_idx)) == NULL) {
            printf("syntax error");
            EXIT()
        }
        list_idx++;
    }
    if (check_correctness(token_list, list_idx)) {
        printf("syntax error");
        EXIT()
    }
    List *stack = make_list();

    for (int i = list_idx - 1; i > -1; i--) {
        push(stack, make_node(token_list[i]));
    }
    List *notation = build_notation(stack, token_list);

    int result = calculate_notation(notation, token_list);
    printf("%d", result);

    return 0;
}

#undef EXIT
