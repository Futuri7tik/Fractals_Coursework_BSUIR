#include "raylib.h"
#include "raymath.h"
#include "functions.h"

// получение цвета дерева пифагора
static Color get_color_tree(const int depth, const TreeParameters* params) {
    const int color_brown[3] = {102, 51, 0}, color_green[3] = {0, 255, 0};
    const float t = 1.0f - (float) depth / (params->depth * 1.0f);

    const unsigned char r = color_brown[0] * (1 - t) + color_green[0] * t,
    g = color_brown[1] * (1 - t) + color_green[1] * t,
    b = color_brown[2] * (1 - t) + color_green[2] * t;
    return (Color){r, g, b, 255};
}

// отрисовка дерева пифагора на экран
void draw_tree(float x_start, float y_start, float length, float angle,
               int depth, TreeParameters* params) {
    depth -= 1;

    // базовый случай рекурсии
    if (depth < 0)
        return;

    // конеченая точка отрезка
    const float x_end = x_start + cosf(angle) * length, y_end = y_start + sinf(angle) * length;

    // отрисовка отрезка
    Color color = get_color_tree(depth, params);
    DrawLine((int) x_start, (int) y_start, (int)x_end, (int) y_end, color);

    // получение параметров для рекурсивных вызовов
    const float new_length = length * params->length_factor,
    angle_left = angle - params->angle,
    angle_right = angle + params->angle;

    // левая ветка
    draw_tree(x_end, y_end, new_length, angle_left, depth, params);

    // правая ветка
    draw_tree(x_end, y_end, new_length, angle_right, depth, params);
}