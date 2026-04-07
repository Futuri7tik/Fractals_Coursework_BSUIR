#include "raylib.h"
#include "raymath.h"
#include "stdio.h"
#include "functions.h"

Color get_color_tree(const int depth, const TreeParameters* params) {
    const int color_brown[3] = {102, 51, 0};
    const int color_green[3] = {0, 255, 0};
    const float t = 1.0f - (float) depth / (params->depth * 1.0f);
    const unsigned char r = color_brown[0] * (1 - t) + color_green[0] * t;
    const unsigned char g = color_brown[1] * (1 - t) + color_green[1] * t;
    const unsigned char b = color_brown[2] * (1 - t) + color_green[2] * t;
    return (Color){r, g, b, 255};
}

void draw_tree(float x_start, float y_start, float length, float angle,
               int depth, TreeParameters* params) {
    depth -= 1;
    if (depth < 0) return;

    float x_end = x_start + cosf(angle) * length;
    float y_end = y_start + sinf(angle) * length;

    Color color = get_color_tree(depth, params);

    // Рисуем линию напрямую (без vertex buffer)
    DrawLine((int) x_start, (int) y_start, (int)x_end, (int) y_end, color);

    float new_length = length * params->length_factor;
    float angle_left = angle - params->angle;
    float angle_right = angle + params->angle;

    // Рекурсия
    draw_tree(x_end, y_end, new_length, angle_left, depth, params);
    draw_tree(x_end, y_end, new_length, angle_right, depth, params);
}