#include "shunting_yard.h"

#include <stdio.h>
#include <stdlib.h>

int get_precedence(char op) {
    int result = 0;
    if (op == '~') result = 3;
    if (op == '*' || op == '/') result = 2;
    if (op == '+' || op == '-') result = 1;
    return result;
}

void pop_to_output(stack_node_t **src, stack_node_t **dst) {
    stack_node_t *n = stack_pop(src);
    stack_push(dst, n->data, n->type);
    free(n);
}

void shunting_yard(stack_node_t **input, stack_node_t **output) {
    stack_node_t *operator_stack = NULL;
    while (!stack_is_empty(*input)) {
        stack_node_t *tok = stack_pop(input);
        if (tok->type == TYPE_NUMBER || tok->type == TYPE_VARIABLE) {
            stack_push(output, tok->data, tok->type);
            free(tok);
        } else if (tok->type == TYPE_FUNCTION) {
            stack_push(&operator_stack, tok->data, tok->type);
            free(tok);
        } else if (tok->type == TYPE_OPERATOR) {
            char operator= *(char *) tok->data;
            while (!stack_is_empty(operator_stack) && operator_stack->type != TYPE_LPAREN &&
                   ((operator_stack->type == TYPE_OPERATOR &&
                     get_precedence(*(char *)operator_stack->data) >= get_precedence(operator)) ||
                    operator_stack->type == TYPE_FUNCTION)) {
                pop_to_output(&operator_stack, output);
            }
            stack_push(&operator_stack, tok->data, tok->type);
            free(tok);
        } else if (tok->type == TYPE_LPAREN) {
            stack_push(&operator_stack, tok->data, tok->type);
            free(tok);
        } else if (tok->type == TYPE_RPAREN) {
            while (!stack_is_empty(operator_stack) && operator_stack->type != TYPE_LPAREN)
                pop_to_output(&operator_stack, output);
            if (!stack_is_empty(operator_stack) && operator_stack->type == TYPE_LPAREN) {
                stack_node_t *lp = stack_pop(&operator_stack);
                free(lp->data);
                free(lp);
            }
            if (!stack_is_empty(operator_stack) && operator_stack->type == TYPE_FUNCTION)
                pop_to_output(&operator_stack, output);
            free(tok->data);
            free(tok);
        }
    }
    while (!stack_is_empty(operator_stack)) pop_to_output(&operator_stack, output);
    *output = stack_reverse(*output);
}