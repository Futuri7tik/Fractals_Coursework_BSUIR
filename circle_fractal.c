#include <math.h>
#include <raylib.h>
#include <stdio.h>

#include "functions.h"

void draw_circle_fractal(float x, float y, float r, int depth, Color color) {
    if (depth <= 0) return;

    DrawCircleLinesV((Vector2) {x, y}, r, color);

    DrawCircleLinesV((Vector2) {x, y + 2 * r}, r, color);
    DrawCircleLinesV((Vector2) {x + 2 * r * cosf(PI / 6), y + 2 * r * sinf(PI / 6)}, r, color);
    DrawCircleLinesV((Vector2) {x + 2 * r * cosf(PI / 6), y - 2 * r * sinf(PI / 6)}, r, color);
    DrawCircleLinesV((Vector2) {x, y - 2 * r}, r, color);
    DrawCircleLinesV((Vector2) {x - 2 * r * cosf(PI / 6), y - 2 * r * sinf(PI / 6)}, r, color);
    DrawCircleLinesV((Vector2) {x - 2 * r * cosf(PI / 6), y + 2 * r * sinf(PI / 6)}, r, color);

    draw_circle_fractal(x, y, r / 3.0f, depth - 1, color);
    draw_circle_fractal(x, y + 2 * r, r / 3.0f, depth - 1, color);
    draw_circle_fractal(x + 2 * r * cosf(PI / 6), y + 2 * r * sinf(PI / 6), r / 3.0f, depth - 1, color);
    draw_circle_fractal(x + 2 * r * cosf(PI / 6), y - 2 * r * sinf(PI / 6), r / 3.0f, depth - 1, color);
    draw_circle_fractal(x, y - 2 * r, r / 3.0f, depth - 1, color);
    draw_circle_fractal(x - 2 * r * cosf(PI / 6), y - 2 * r * sinf(PI / 6), r / 3.0f, depth - 1, color);
    draw_circle_fractal(x - 2 * r * cosf(PI / 6), y + 2 * r * sinf(PI / 6), r / 3.0f, depth - 1, color);
}