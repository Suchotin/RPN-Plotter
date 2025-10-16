#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

#define TYPE_NUMBER 0
#define TYPE_OPERATOR 1
#define TYPE_FUNCTION 2
#define TYPE_LPAREN 3
#define TYPE_RPAREN 4
#define TYPE_VARIABLE 5

int my_isdigit(int c) { return c >= '0' && c <= '9'; }

int my_isalpha(int c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

int my_isspace(int c) { return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v'; }

int is_function(const char *s) {
    return strcmp(s, "sin") == 0 || strcmp(s, "cos") == 0 || strcmp(s, "tan") == 0 || strcmp(s, "ctg") == 0 ||
           strcmp(s, "ln") == 0 || strcmp(s, "sqrt") == 0;
}

static int is_number_char(int c) { return my_isdigit(c) || c == '.'; }

static char *read_token(int first, int (*cond)(int)) {
    size_t cap = 8;
    char *buf = malloc(cap);
    char *result = NULL;
    if (buf) {
        size_t len = 0;
        int c;
        buf[len++] = (char)first;
        while ((c = getchar()) != EOF && cond(c)) {
            if (len + 1 >= cap) {
                cap *= 2;
                char *new_buf = realloc(buf, cap);
                if (!new_buf) {
                    free(buf);
                    buf = NULL;
                    break;
                }
                buf = new_buf;
            }
            if (buf) {
                buf[len++] = (char)c;
            }
        }
        if (buf) {
            buf[len] = '\0';
            if (c != EOF) ungetc(c, stdin);
            result = buf;
        }
    }
    return result;
}

static int push_number(stack_node_t **st, char *buf) {
    int result = 1;
    double *num = malloc(sizeof(double));
    if (!num) {
        result = 0;
    } else {
        *num = atof(buf);
        free(buf);
        if (!*st) {
            *st = stack_init(num, TYPE_NUMBER);
        } else {
            stack_push(st, num, TYPE_NUMBER);
        }
    }
    return result;
}

static int push_identifier(stack_node_t **st, char *buf, int *last_type) {
    int ok = 1;
    if (is_function(buf)) {
        if (!*st)
            *st = stack_init(buf, TYPE_FUNCTION);
        else
            stack_push(st, buf, TYPE_FUNCTION);
        *last_type = TYPE_FUNCTION;
    } else if (strcmp(buf, "x") == 0) {
        if (!*st)
            *st = stack_init(buf, TYPE_VARIABLE);
        else
            stack_push(st, buf, TYPE_VARIABLE);
        if (*last_type == TYPE_NUMBER || *last_type == TYPE_VARIABLE) ok = 0;
        *last_type = TYPE_VARIABLE;
    } else {
        ok = 0;
        free(buf);
    }
    return ok;
}

static int push_operator(stack_node_t **st, char c, int *last_type) {
    int result = 1;
    char *op = NULL;
    if (c == '-' && (*last_type != TYPE_NUMBER && *last_type != TYPE_VARIABLE && *last_type != TYPE_RPAREN)) {
        c = '~';
    }
    if ((*last_type != TYPE_NUMBER && *last_type != TYPE_VARIABLE && *last_type != TYPE_RPAREN) && c != '~') {
        result = 0;
    } else if (c == '~' &&
               (*last_type == TYPE_NUMBER || *last_type == TYPE_VARIABLE || *last_type == TYPE_RPAREN)) {
        result = 0;
    } else {
        op = (char *)malloc(sizeof(char));
        if (!op) {
            result = 0;
        } else {
            *op = c;
            if (!*st)
                *st = stack_init(op, TYPE_OPERATOR);
            else
                stack_push(st, op, TYPE_OPERATOR);
            *last_type = TYPE_OPERATOR;
        }
    }
    return result;
}

static int push_paren(stack_node_t **st, char c, int *paren_balance, int *last_type) {
    int ok = 1;
    char *p = malloc(2);
    p[0] = c;
    p[1] = '\0';
    if (c == '(') {
        if (*last_type == TYPE_NUMBER || *last_type == TYPE_VARIABLE) {
            ok = 0;
            free(p);
        } else {
            if (!*st)
                *st = stack_init(p, TYPE_LPAREN);
            else
                stack_push(st, p, TYPE_LPAREN);
            (*paren_balance)++;
            *last_type = TYPE_LPAREN;
        }
    } else {
        if (*paren_balance == 0) {
            ok = 0;
            free(p);
        } else {
            if (*last_type == TYPE_OPERATOR || *last_type == TYPE_FUNCTION || *last_type == TYPE_LPAREN) {
                ok = 0;
                free(p);
            } else {
                if (!*st)
                    *st = stack_init(p, TYPE_RPAREN);
                else
                    stack_push(st, p, TYPE_RPAREN);
                (*paren_balance)--;
                *last_type = TYPE_RPAREN;
            }
        }
    }
    return ok;
}

stack_node_t *parse_input(void) {
    stack_node_t *input = NULL;
    int c, paren_balance = 0, last_type = -1;
    int ok = 1;
    stack_node_t *result = NULL;
    while ((c = getchar()) != EOF && c != '\n' && ok) {
        if (my_isspace(c)) {
        } else if (my_isdigit(c) || c == '.') {
            char *buf = read_token(c, is_number_char);
            ok = push_number(&input, buf);
            if (last_type == TYPE_NUMBER || last_type == TYPE_VARIABLE) ok = 0;
            last_type = TYPE_NUMBER;
        } else if (my_isalpha(c)) {
            char *buf = read_token(c, my_isalpha);
            ok = push_identifier(&input, buf, &last_type);
        } else if (strchr("+-*/~", c)) {
            ok = push_operator(&input, c, &last_type);
        } else if (c == '(' || c == ')') {
            ok = push_paren(&input, c, &paren_balance, &last_type);
        } else {
            ok = 0;
        }
    }
    if (paren_balance != 0) ok = 0;
    if (last_type == TYPE_OPERATOR || last_type == TYPE_FUNCTION || last_type == TYPE_LPAREN) ok = 0;
    if (last_type == -1) ok = 0;
    if (ok) {
        result = input ? stack_reverse(input) : NULL;
    } else {
        stack_free(&input);
        result = NULL;
    }
    return result;
}