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

void init_random_config(FractalParameters* params, Camera2D* cam, AppState* type) {
    init_random();
    init_fractals_parameters(params);

    *type = STATE_TREE + rand() % 6;
    cam->target = (Vector2){WIDTH/2.0f, HEIGHT/2.0f};

    switch (*type) {
        case STATE_TREE:
            params->tree.depth = 8 + rand() % 8;
            params->tree.angle_degrees = 15.0f + rand() % 400 / 10.0f;
            params->tree.angle = params->tree.angle_degrees * DEG2RAD;
            params->tree.length_factor = 0.6f + rand() % 25 / 100.0f;
            break;
        case STATE_CARPET:
            params->carpet.depth = 3 + rand() % 5; // 3..6
            params->carpet.red = 50 + rand() % 205;
            params->carpet.green = 50 + rand() % 205;
            params->carpet.blue = 50 + rand() % 205;
            break;
        case STATE_TRIANGLE:
            params->triangle.depth = 5 + rand() % 6; // 5..10
            params->triangle.red = rand() % 255;
            params->triangle.green = rand() % 255;
            params->triangle.blue = rand() % 255;
            break;
        case STATE_MANDELBROT:
            cam->target = (Vector2){WIDTH/2.0f - 300, HEIGHT/2.0f};
            params->mandelbrot.iterations = 100 + rand() % 400; // 100..500
            params->mandelbrot.red = 5 + rand() % 15;
            params->mandelbrot.green = 5 + rand() % 15;
            params->mandelbrot.blue = 5 + rand() % 15;
            break;
        case STATE_JULIA:
            params->julia.iterations = 100 + rand() % 400;
            params->julia.re_c = -1.0f + rand() % 2000 / 1000.0f;
            params->julia.im_c = -1.0f + rand() % 2000 / 1000.0f;
            params->julia.red = 5 + rand() % 15;
            params->julia.green = 5 + rand() % 15;
            params->julia.blue = 5 + rand() % 15;
            break;
        case STATE_CIRCLE:
            params->circle.depth = 1 + rand() % params->circle.max_depth;
            params->circle.red = rand() % 255;
            params->circle.green = rand() % 255;
            params->circle.blue = rand() % 255;
            break;
    }
}