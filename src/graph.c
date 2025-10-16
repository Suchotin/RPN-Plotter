#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calculate.h"
#include "parser.h"
#include "render.h"
#include "shunting_yard.h"
#include "stack.h"

int main(void) {
    stack_node_t* input = parse_input();
    int result = 0;
    if (!input) {
        printf("n/a");
        stack_free(&input);
        result = 1;
    } else {
        stack_node_t* output = NULL;
        shunting_yard(&input, &output);
        if (!output) {
            printf("n/a");
            result = 1;
        } else {
            render_graph(output);
            stack_free(&output);
            result = 0;
        }
    }
    getchar();
    return result;
}