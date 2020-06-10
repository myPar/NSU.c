#include <stdio.h>
#include <stdlib.h>
#include "structures.h"

int check_operator_open_parenthesis(Token **token_list, int cur_idx) {
    Token *next_token = token_list[cur_idx + 1];

    if (next_token->type != OPEN_PARENTHESIS && next_token->type != IDENT) {
        return 1;
    }
    return 0;
}
int check_ident_close_parenthesis(Token **token_list, int cur_idx) {
    Token *next_token = token_list[cur_idx + 1];

    if (next_token->type != CLOSE_PARENTHESIS && next_token->type != OPERATOR) {
        return 1;
    }
    return 0;
}
// check the correctness of input expression
// based on infix expression notation syntax
int check_correctness(Token **token_list, int list_size) {
    if (token_list[0]->type == OPERATOR || token_list[0]->type == CLOSE_PARENTHESIS) {
        return 1;
    }
    for (int i  = 0; i < list_size - 1; i++) {
        switch(token_list[i]->type) {
            case OPERATOR:
            case OPEN_PARENTHESIS:
                if (check_operator_open_parenthesis(token_list, i)) {
                    return 1;
                }
                break;
            case IDENT:
            case CLOSE_PARENTHESIS:
                if (check_ident_close_parenthesis(token_list, i)) {
                    return 1;
                }
                break;
        }
    }
    if (token_list[list_size - 1]->type == OPERATOR || token_list[list_size - 1]->type == OPEN_PARENTHESIS) {
        return 1;
    }
    return 0;
}
