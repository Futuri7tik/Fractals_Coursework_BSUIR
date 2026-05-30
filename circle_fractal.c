#include <math.h>
#include <raylib.h>
#include "functions.h"

// отрисовка кругового фрактала
void draw_circle_fractal(float x, float y, float r, int depth, Color color) {
    // базовый случай рекурсии
    if (depth <= 0)
        return;

    // отрисовка центрального круга
    DrawCircleLinesV((Vector2) {x, y}, r, color);

    // отрисовка кругов между средней и большей окружностями
    DrawCircleLinesV((Vector2) {x, y + 2 * r}, r, color);
    DrawCircleLinesV((Vector2) {x + 2 * r * cosf(30 * DEG2RAD), y + 2 * r * sinf(30 * DEG2RAD)}, r, color);
    DrawCircleLinesV((Vector2) {x + 2 * r * cosf(30 * DEG2RAD), y - 2 * r * sinf(30 * DEG2RAD)}, r, color);
    DrawCircleLinesV((Vector2) {x, y - 2 * r}, r, color);
    DrawCircleLinesV((Vector2) {x - 2 * r * cosf(30 * DEG2RAD), y - 2 * r * sinf(30 * DEG2RAD)}, r, color);
    DrawCircleLinesV((Vector2) {x - 2 * r * cosf(30 * DEG2RAD), y + 2 * r * sinf(30 * DEG2RAD)}, r, color);

    // рекурсивный вызов отрисовки для остальных окружностей
    draw_circle_fractal(x, y, r / 3.0f, depth - 1, color);
    draw_circle_fractal(x, y + 2 * r, r / 3.0f, depth - 1, color);
    draw_circle_fractal(x + 2 * r * cosf(30 * DEG2RAD), y + 2 * r * sinf(30 * DEG2RAD), r / 3.0f, depth - 1, color);
    draw_circle_fractal(x + 2 * r * cosf(30 * DEG2RAD), y - 2 * r * sinf(30 * DEG2RAD), r / 3.0f, depth - 1, color);
    draw_circle_fractal(x, y - 2 * r, r / 3.0f, depth - 1, color);
    draw_circle_fractal(x - 2 * r * cosf(30 * DEG2RAD), y - 2 * r * sinf(30 * DEG2RAD), r / 3.0f, depth - 1, color);
    draw_circle_fractal(x - 2 * r * cosf(30 * DEG2RAD), y + 2 * r * sinf(30 * DEG2RAD), r / 3.0f, depth - 1, color);
}