#include "raylib.h"
#include "stdio.h"
#include "functions.h"

void draw_square(float x, float y, float length, Color color) {
    DrawRectangle(x, y, length, length, color);
}

Color get_color_carpet(const int depth, CarpetParameters* params) {
    const int color_1[3] = {0, 0, 0};
    const int color_2[3] = {0, 0, 153};
    const float t = 1.0f - (float) depth / (float) params->max_depth;
    const unsigned char r = color_1[0] * (1 - t) + color_2[0] * t;
    const unsigned char g = color_1[1] * (1 - t) + color_2[1] * t;
    const unsigned char b = color_1[2] * (1 - t) + color_2[2] * t;
    return (Color){r, g, b, 255};
}

void draw_carpet(float x_left, float y_left, float length,
                 int depth, CarpetParameters* params) {
    depth -= 1;
    if (depth < 0) return;

    float new_length = length / 3.0f;
    float x_draw = x_left + new_length;
    float y_draw = y_left + new_length;

    Color color = get_color_carpet(depth, params);
    draw_square(x_draw, y_draw, new_length, color);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i != 1 || j != 1) {
                draw_carpet(x_left + (float) i * new_length, y_left + (float) j * new_length,
                           new_length, depth, params);
            }
        }
    }
}