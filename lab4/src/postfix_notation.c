#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "structures.h"

#define EXIT()                      \
    printf("syntax error");         \
    free_list(operation_stack);     \
    free_list(output_stack);        \
    free_list(input_list);          \
    free(token_list);               \
    return NULL;

List *build_notation(List *input_list, Token **token_list) {
    List *output_stack = make_list();
    List *operation_stack = make_list();
    int parenthesis_count = 0;
    bool is_complete;

    while (input_list->size > 0) {
        Node *cur_node = pop(input_list);

        switch (cur_node->token->type) {
            case IDENT:
                push(output_stack, cur_node);
                break;
            case OPERATOR:
                // remove all operators with equal or less priority up to '('
                // and push them to the output stack
                while (operation_stack->size > 0) {
                    if ((operation_stack->tail)->token->type == OPEN_PARENTHESIS) {
                        break;
                    }
                    if (get_priority(cur_node->token) > get_priority(operation_stack->tail->token)) {
                        break;
                    }
                    push(output_stack, pop(operation_stack));
                }
                // push new operator in the stack
                push(operation_stack, cur_node);
                break;
            case OPEN_PARENTHESIS:
                parenthesis_count++;
                push(operation_stack, cur_node);
                break;
            case CLOSE_PARENTHESIS:
                parenthesis_count--;
                is_complete = false;
                int cur_stack_size = operation_stack->size;

                for (int i = 0; i < cur_stack_size; i++) {
                    if ((operation_stack->tail)->token->type == OPEN_PARENTHESIS) {
                        free_node(pop(operation_stack));
                        is_complete = true;
                        break;
                    }
                    push(output_stack, pop(operation_stack));
                }
                if (!is_complete) {
                    // disturbed parenthesis balance
                    EXIT()
                }
                // free memory from unused node
                free_node(cur_node);
                break;
        }
    }
    if (parenthesis_count != 0) {
        // disturbed parenthesis balance
        EXIT()
    }
    if (operation_stack->size > 0) {
        // add remaining data in the output stack
        int cur_stack_size = operation_stack->size;

        for (int i = 0; i < cur_stack_size; i++) {
            push(output_stack, pop(operation_stack));
        }
    }
    // free empty lists
    free(operation_stack);
    free(input_list);

    return output_stack;
}

#undef EXIT

Node *get_operator_pointer(List *notation) {
    Node *cur_node = notation->head;

    while (cur_node != NULL && cur_node->token->type != OPERATOR) {
        cur_node = cur_node->next;
    }
    return cur_node;
}

// input expression has the correct infix syntax
// so we just have to calculate an expression without
// exception checking (besides division by zero checking)
int *calculate_notation(List *postfix_notation, Token **token_list, int *res_pointer) {

    while (postfix_notation->size > 1) {
        Node *operator = get_operator_pointer(postfix_notation);
        Node *operand1 = operator->prev->prev;
        Node *operand2 = operator->prev;
        Node *result = calculate(operator, operand1, operand2);

        if (result == NULL) {
            printf("division by zero");
            free_list(postfix_notation);
            free(token_list);
            return NULL;
        }
        // change pointers
        if (operand1->prev != NULL) {
            result->prev = operand1->prev;
            operand1->prev->next = result;
        }
        else {
            // move head pointer
            postfix_notation->head = result;
        }
        if (operator->next != NULL) {
            result->next = operator->next;
            operator->next->prev = result;
        }
        else {
            // move tail pointer
            postfix_notation->tail = result;
        }
        // free memory from cut nodes
        free_node(operand1);
        free_node(operand2);
        free_node(operator);
        postfix_notation->size -= 2;
    }
    assert(postfix_notation->head->token->type == IDENT);
    int res = atoi(postfix_notation->head->token->value);
    free_list(postfix_notation);
    free(token_list);

    *res_pointer = res;
    return res_pointer;
}
