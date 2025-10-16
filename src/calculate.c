#include "calculate.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

static int push_number(stack_node_t **work, double val) {
    int ok = 1;
    double *v = (double *)malloc(sizeof(double));
    if (!v)
        ok = 0;
    else {
        *v = val;
        stack_push(work, v, TYPE_NUMBER);
    }
    return ok;
}

static int apply_func(stack_node_t **work, const char *f) {
    int ok = 1;
    stack_node_t *a = stack_pop(work);
    if (!a || !a->data) {
        if (a) free(a);
        ok = 0;
    } else {
        double x = *(double *)a->data;
        free(a->data);
        free(a);
        double r = NAN;
        if (!strcmp(f, "sin"))
            r = sin(x);
        else if (!strcmp(f, "cos"))
            r = cos(x);
        else if (!strcmp(f, "tan"))
            r = tan(x);
        else if (!strcmp(f, "ctg"))
            r = cos(x) / (sin(x));
        else if (!strcmp(f, "ln"))
            r = (x <= 0.0) ? NAN : log(x);
        else if (!strcmp(f, "sqrt"))
            r = (x < 0.0) ? NAN : sqrt(x);
        if (!isfinite(r))
            ok = 0;
        else if (!push_number(work, r))
            ok = 0;
    }
    return ok;
}

static int apply_op(stack_node_t **work, char op) {
    int ok = 1;
    stack_node_t *b = stack_pop(work);
    if (!b || !b->data) {
        if (b) free(b);
        ok = 0;
    } else {
        double y = *(double *)b->data;
        free(b->data);
        free(b);
        double x = 0.0;
        if (op != '~') {
            stack_node_t *a = stack_pop(work);
            if (!a || !a->data) {
                if (a) free(a);
                ok = 0;
            } else {
                x = *(double *)a->data;
                free(a->data);
                free(a);
            }
        }
        if (ok) {
            double r = NAN;
            if (op == '+')
                r = x + y;
            else if (op == '-')
                r = x - y;
            else if (op == '*')
                r = x * y;
            else if (op == '/')
                r = (y == 0.0) ? NAN : (x / y);
            else if (op == '~')
                r = -y;
            if (!isfinite(r))
                ok = 0;
            else if (!push_number(work, r))
                ok = 0;
        }
    }
    return ok;
}

double eval_rpn(stack_node_t **out, double x_value, int *calc_err) {
    stack_node_t *work = NULL;
    *calc_err = 0;
    for (stack_node_t *t = *out; t && *calc_err == 0; t = t->next) {
        if (t->type == TYPE_NUMBER) {
            if (!push_number(&work, *(double *)t->data)) *calc_err = 2;
        } else if (t->type == TYPE_VARIABLE) {
            if (!push_number(&work, x_value)) *calc_err = 2;
        } else if (t->type == TYPE_FUNCTION) {
            if (!apply_func(&work, (const char *)t->data)) *calc_err = 2;
        } else if (t->type == TYPE_OPERATOR) {
            if (!apply_op(&work, *(char *)t->data)) *calc_err = 2;
        }
    }
    double result = NAN;
    if (*calc_err == 0 && work && !work->next) result = *(double *)work->data;
    if (*calc_err == 0 && (!work || work->next)) *calc_err = 2;
    while (!stack_is_empty(work)) {
        stack_node_t *n = stack_pop(&work);
        if (n->data) free(n->data);
        free(n);
    }
    return result;
}