#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H
#include "stack.h"

int get_precedence(char op);
void pop_to_output(stack_node_t **src, stack_node_t **dst);
void shunting_yard(stack_node_t **input, stack_node_t **output);

#endif