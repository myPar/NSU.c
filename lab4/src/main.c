#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "structures.h"
#include "expression_correctness.h"
#include "postfix_notation.h"

int main(int argc, char *argv[]) {
    FILE *input = NULL;
    char input_string[1024] = {0};

    size_t expression_len;
    int cur_idx = 0;

    input = set_input(argc, argv);
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
    Token *token_list = (Token*) malloc(sizeof(Token) * expression_len);
    int list_idx = 0;

    while (cur_idx < expression_len) {
        token_list[list_idx] = *get_token(input_string, &cur_idx);
        list_idx++;
    }
    check_correctness(token_list, list_idx);
    List *stack = make_list();

    for (int i = list_idx - 1; i > -1; i--) {
        push(stack, make_node(&(token_list[i])));
    }
    List *notation = build_notation(stack);
    int result = calculate_notation(notation);
    printf("%d", result);

    free(token_list);
    return 0;
}