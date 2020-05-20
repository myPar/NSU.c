#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
// print exception message and
// free memory from token array
void EXIT(Token *token_list) {
    printf("syntax error");
    free(token_list);
    exit(0);
}
void check_operator_open_parenthesis(Token *token_list, int cur_idx) {
    Token next_token = token_list[cur_idx + 1];

    if (next_token.type != OPEN_PARENTHESIS && next_token.type != IDENT) {
        EXIT(token_list);
    }
}
void check_ident_close_parenthesis(Token *token_list, int cur_idx) {
    Token next_token = token_list[cur_idx + 1];

    if (next_token.type != CLOSE_PARENTHESIS && next_token.type != OPERATOR) {
        EXIT(token_list);
    }
}
// check the correctness of input expression
// based on infix expression notation syntax
void check_correctness(Token *token_list, int list_size) {
    if (token_list[0].type == OPERATOR || token_list[0].type == CLOSE_PARENTHESIS) {
        EXIT(token_list);
    }
    for (int i  = 0; i < list_size - 1; i++) {
        switch(token_list[i].type) {
            case OPERATOR:
            case OPEN_PARENTHESIS:
                check_operator_open_parenthesis(token_list, i);
                break;
            case IDENT:
            case CLOSE_PARENTHESIS:
                check_ident_close_parenthesis(token_list, i);
                break;
        }
    }
    if (token_list[list_size - 1].type == OPERATOR || token_list[list_size - 1].type == OPEN_PARENTHESIS) {
        EXIT(token_list);
    }
}