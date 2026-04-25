#include <math.h>
#include "raylib.h"
#include "functions.h"

typedef struct DragonParameters {
    float depth;
    float x;
    float y;
    float angle;
    float length;
    int max_depth;
    float start_length;
    float red;
    float green;
    float blue;
} DragonParameters;

void draw_x(DragonParameters* params, int depth);
void draw_y(DragonParameters* params, int depth);
void draw_dragon(DragonParameters* params);
void draw_f(DragonParameters* params);

void draw_f(DragonParameters* params) {
    float next_x = params->x + cosf(params->angle * DEG2RAD) * params->length;
    float next_y = params->y + sinf(params->angle * DEG2RAD) * params->length;
    DrawLineEx((Vector2){params->x, params->y}, (Vector2){next_x, next_y}, 1.0f,
        (Color){params->red, params->green, params->blue, 255});
    params->x = next_x;
    params->y = next_y;
}

// y -> - fx - y
void draw_y(DragonParameters* params, int depth) {
    if (depth <= 0)
        return;

    params->angle -= 90;
    draw_f(params);

    draw_x(params, depth - 1);

    params->angle -= 90;
    draw_y(params, depth - 1);
}

// x -> x + yf +
void draw_x(DragonParameters* params, int depth) {
    if (depth <= 0)
        return;

    draw_x(params, depth - 1);
    params->angle += 90;

    draw_y(params, depth - 1);
    draw_f(params);
    params->angle += 90;
}

void draw_dragon(DragonParameters* params) {
    draw_f(params);
    draw_x(params, 12);
}



