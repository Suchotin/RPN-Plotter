#include <math.h>
#include <stdio.h>

#include "calculate.h"

#define HEIGHT 25
#define WIDTH 80
#define X_MIN 0.0
#define X_MAX (4 * 3.1415926535897932)
#define Y_MIN -1.0
#define Y_MAX 1.0

void render_graph(stack_node_t *output) {
    char canvas[HEIGHT][WIDTH];
    int valid = 0;
    int err = 0;
    for (int r = 0; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            canvas[r][c] = '.';
        }
    }
    for (int col = 0; col < WIDTH; col++) {
        double x = X_MIN + (X_MAX - X_MIN) * col / (WIDTH - 1.0);
        double y = -eval_rpn(&output, x, &err);
        if (!err) {
            if (isfinite(y)) {
                int row = (int)lround((Y_MAX - y) / (Y_MAX - Y_MIN) * (HEIGHT - 1));
                if (row >= 0 && row < HEIGHT) {
                    canvas[row][col] = '*';
                    valid = 1;
                }
            }
        } else {
            err = 0;
        }
    }
    if (!valid) {
        printf("n/a");
    } else {
        for (int r = 0; r < HEIGHT; r++) {
            for (int c = 0; c < WIDTH; c++) {
                putchar(canvas[r][c]);
            }
            putchar('\n');
        }
    }
}