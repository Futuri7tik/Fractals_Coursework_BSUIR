#include "raylib.h"
#include "functions.h"
#include "raymath.h"
#define SQRT3 0.866025f

void draw_triangle_base(float x, float y, float length, Color color) {
    float height = length * sqrtf(3.0f) / 2.0f;
    DrawTriangle(
        (Vector2){x, y},                          // Левый нижний
        (Vector2){x + length, y},                 // Правый нижний
        (Vector2){x + length / 2.0f, y - height}, // Верхний
        color
    );
}

// Центральный перевёрнутый треугольник (синий)
void draw_center_triangle(float x, float y, float length, Color color) {
    float height = length * sqrtf(3.0f) / 2.0f;
    DrawTriangle(
        (Vector2){x + length / 2.0f, y},                     // Середина основания
        (Vector2){x + length / 4.0f, y - height / 2.0f},     // Середина левой стороны
        (Vector2){x + 3.0f * length / 4.0f, y - height / 2.0f}, // Середина правой стороны
        color
    );
}

Color get_color_triangle(const int depth, TriangleParameters* params) {
    const int color_1[3] = {0, 0, 0};
    const int color_2[3] = {0, 0, 153};

    // Защита от деления на 0
    if (params->max_depth <= 0) {
        return (Color){0, 0, 153, 255};
    }

    const float t = 1.0f - (float)depth / (float)params->max_depth;
    const unsigned char r = color_1[0] * (1.0f - t) + color_2[0] * t;
    const unsigned char g = color_1[1] * (1.0f - t) + color_2[1] * t;
    const unsigned char b = color_1[2] * (1.0f - t) + color_2[2] * t;
    return (Color){r, g, b, 255};
}

void draw_sierpinski_triangle(float x, float y, float length,
                              int depth, TriangleParameters* params) {
    if (depth <= 0) return;

    float height = length * sqrtf(3.0f) / 2.0f;
    float h2 = height / 2.0f;
    float l2 = length / 2.0f;
    float l4 = length / 4.0f;

    // 1. Рисуем центральный (перевернутый) треугольник
    Color color = get_color_triangle(depth, params);

    DrawTriangle(
        (Vector2){ x + l4, y - h2 },
        (Vector2){ x + l2, y },
        (Vector2){ x + 3.0f * l4, y - h2 },
        color
    );

    // 2. Рекурсия для трех угловых треугольников
    // Левый нижний
    draw_sierpinski_triangle(x, y, l2, depth - 1, params);
    // Правый нижний
    draw_sierpinski_triangle(x + l2, y, l2, depth - 1, params);
    // Верхний
    draw_sierpinski_triangle(x + l4, y - h2, l2, depth - 1, params);
}