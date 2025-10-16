#ifndef CALCULATE_H
#define CALCULATE_H
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

double eval_rpn(stack_node_t **out, double x_value, int *err);

#endif