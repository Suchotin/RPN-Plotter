#include "stack.h"

#include <stdlib.h>

stack_node_t *stack_init(void *data, int type) {
    stack_node_t *node = (stack_node_t *)malloc(sizeof(stack_node_t));
    if (node) {
        node->data = data;
        node->type = type;
        node->next = NULL;
    }
    return node;
}

int stack_is_empty(const stack_node_t *top) { return top == NULL; }

void stack_push(stack_node_t **top, void *data, int type) {
    stack_node_t *node = (stack_node_t *)malloc(sizeof(stack_node_t));
    if (!node) return;
    node->data = data;
    node->type = type;
    node->next = *top;
    *top = node;
}

stack_node_t *stack_pop(stack_node_t **top) {
    stack_node_t *node = NULL;
    if (!stack_is_empty(*top)) {
        node = *top;
        *top = (*top)->next;
    }
    return node;
}

stack_node_t *stack_reverse(stack_node_t *top) {
    stack_node_t *prev = NULL, *curr = top, *next = NULL;
    while (curr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}

void stack_free(stack_node_t **top) {
    while (!stack_is_empty(*top)) {
        stack_node_t *node = stack_pop(top);
        free(node->data);
        free(node);
    }
}