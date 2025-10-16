#ifndef STACK_H
#define STACK_H
#define TYPE_NUMBER 0
#define TYPE_OPERATOR 1
#define TYPE_FUNCTION 2
#define TYPE_LPAREN 3
#define TYPE_RPAREN 4
#define TYPE_VARIABLE 5

typedef struct stack_node {
    void *data;
    int type;
    struct stack_node *next;
} stack_node_t;

stack_node_t *stack_init(void *data, int type);
void stack_push(stack_node_t **top, void *data, int type);
stack_node_t *stack_pop(stack_node_t **top);
int stack_is_empty(const stack_node_t *top);
stack_node_t *stack_reverse(stack_node_t *top);
void stack_free(stack_node_t **top);

#endif