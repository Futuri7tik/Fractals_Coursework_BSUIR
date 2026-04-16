#include <math.h>
#include <string.h>
#include "functions.h"
#include "raylib.h"
#include "raygui.h"
#include "stdlib.h"
#include "time.h"

static bool random_initialized = false;
void init_random(void) {
    if (!random_initialized) {
        srand((unsigned int)time(NULL));
        random_initialized = true;
    }
}

void init_random_config(FractalParameters* params, AppState* type) {
    init_random();
    init_fractals_parameters(params);

    *type = STATE_TREE + rand() % 5;

    switch (*type) {
        case STATE_TREE:
            params->tree.depth = 8 + rand() % 8;
            params->tree.angle_degrees = 15.0f + rand() % 400 / 10.0f;
            params->tree.angle = params->tree.angle_degrees * DEG2RAD;
            params->tree.length_factor = 0.6f + rand() % 25 / 100.0f;
            break;
        case STATE_CARPET: // Sierpinski Carpet
            params->carpet.depth = 3 + rand() % 5; // 3..6
            params->carpet.red = 50 + rand() % 205;
            params->carpet.green = 50 + rand() % 205;
            params->carpet.blue = 50 + rand() % 205;
            break;
        case STATE_TRIANGLE: // Sierpinski Triangle
            params->triangle.depth = 5 + rand() % 6; // 5..10
            params->triangle.red = rand() % 255;
            params->triangle.green = rand() % 255;
            params->triangle.blue = rand() % 255;
            break;
        case STATE_MANDELBROT: // Mandelbrot
            params->mandelbrot.iterations = 100 + rand() % 400; // 100..500
            params->mandelbrot.red = 5 + rand() % 15;
            params->mandelbrot.green = 5 + rand() % 15;
            params->mandelbrot.blue = 5 + rand() % 15;
            break;
        case STATE_JULIA: // Julia
            params->julia.iterations = 100 + rand() % 400;
            params->julia.re_c = -1.0f + rand() % 2000 / 1000.0f;
            params->julia.im_c = -1.0f + rand() % 2000 / 1000.0f;
            params->julia.red = 5 + rand() % 15;
            params->julia.green = 5 + rand() % 15;
            params->julia.blue = 5 + rand() % 15;
            break;
    }
}