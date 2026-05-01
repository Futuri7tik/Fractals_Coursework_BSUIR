#include <math.h>
#include <string.h>
#include "functions.h"
#include "raylib.h"
#include "raygui.h"
#include "stdlib.h"
#include "time.h"

void init_random_config(FractalParameters* params, Camera2D* cam, AppState* type) {
    init_fractals_parameters(params);

    *type = STATE_MANDELBROT + GetRandomValue(0, 8);
    cam->target = (Vector2){WIDTH/2.0f, HEIGHT/2.0f};

    switch (*type) {
        case STATE_TREE:
            params->tree.depth = 8 + GetRandomValue(0,8);
            params->tree.angle_degrees = 15.0f + (float) GetRandomValue(0,39);
            params->tree.angle = params->tree.angle_degrees * DEG2RAD;
            params->tree.length_factor = 0.6f + (float) GetRandomValue(0,24) / 100.0f;
            break;
        case STATE_CARPET:
            params->carpet.depth = 3 + GetRandomValue(0, 3); // 3..6
            params->carpet.red = 50 + GetRandomValue(0, 205);
            params->carpet.green = 50 + GetRandomValue(0, 205);
            params->carpet.blue = 50 + GetRandomValue(0, 205);
            break;
        case STATE_TRIANGLE:
            params->triangle.depth = 5 + GetRandomValue(0, 5);
            params->triangle.red = GetRandomValue(0, 255);
            params->triangle.green = GetRandomValue(0, 255);
            params->triangle.blue = GetRandomValue(0, 255);
            break;
        case STATE_MANDELBROT:
            cam->target = (Vector2){WIDTH/2.0f - 300, HEIGHT/2.0f};
            params->mandelbrot.iterations = 100 + GetRandomValue(0, 400); // 100..500
            params->mandelbrot.red = 5 + GetRandomValue(0, 15);
            params->mandelbrot.green = 5 + GetRandomValue(0, 15);
            params->mandelbrot.blue = 5 + GetRandomValue(0, 15);
            break;
        case STATE_JULIA:
            params->julia.iterations = 100 + GetRandomValue(0, 400);
            params->julia.re_c = -1.0f + GetRandomValue(0, 2000) / 1000.0f;
            params->julia.im_c = -1.0f + GetRandomValue(0, 2000) / 1000.0f;
            params->julia.red = 5 + GetRandomValue(0, 15);
            params->julia.green = 5 + GetRandomValue(0, 15);
            params->julia.blue = 5 + GetRandomValue(0, 15);
             break;
        case STATE_CIRCLE:
            params->circle.depth = 1.0f + GetRandomValue(0, params->circle.max_depth - 1);
            params->circle.red = GetRandomValue(0, 255);
            params->circle.green = GetRandomValue(0, 255);
            params->circle.blue = GetRandomValue(0, 255);
            break;
        case STATE_FERN:
            params->fern.iterations = 100000.0f + GetRandomValue(0, 399999);
            params->fern.prob1 = GetRandomValue(0, 100);
            params->fern.prob2 = GetRandomValue(0, 100);
            params->fern.prob3 = GetRandomValue(0, 100);
            params->fern.prob4 = GetRandomValue(0, 100);
            break;
        case STATE_NEWTON:
            params->newton.iterations = 100 + GetRandomValue(0, 400);
            params->newton.red = GetRandomValue(0, 255);
            params->newton.green = GetRandomValue(0, 255);
            params->newton.blue = GetRandomValue(0, 255);
            params->newton.gradient_r = GetRandomValue(0, 255);
            params->newton.gradient_g = GetRandomValue(0, 255);
            params->newton.gradient_b = GetRandomValue(0, 255);
            break;
        case STATE_DRAGON:
            params->dragon.depth = GetRandomValue(2, params->dragon.max_depth - 3);
            params->dragon.angle_tilt = GetRandomValue(0, 90);
            params->dragon.length = GetRandomValue(0, 50);
            params->dragon.red = GetRandomValue(0, 255);
            params->dragon.green = GetRandomValue(0, 255);
            params->dragon.blue = GetRandomValue(0, 255);
            break;
    }
}