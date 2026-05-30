#include "raylib.h"
#include "functions.h"

// отрисовка базового треугольника
void draw_triangle_base(float x, float y, float length, Color color) {
    float height = length * SQRT3 / 2.0f;
    DrawTriangle(
        (Vector2){x, y},
        (Vector2){x + length, y},
        (Vector2){x + length / 2.0f, y - height},
        color
    );
}

// отрисовка треугольника серпинского на экран
void draw_sierpinski_triangle(float x, float y, float length, int depth, TriangleParameters* params) {
    // базовый случай рекурсии
    if (depth <= 0)
        return;

    // высота текущего треугольника
    float height = length * SQRT3 / 2.0f;

    // отрисовка среднего треугольника
    DrawTriangle((Vector2){x + length / 4.0f, y - height / 2.0f},(Vector2){x + length / 2.0f, y},
    (Vector2){x + 3.0f * length / 4.0f, y - height / 2.0f}, BLACK);

    // левый нижний
    draw_sierpinski_triangle(x, y, length / 2.0f, depth - 1, params);

    // правый нижний
    draw_sierpinski_triangle(x + length / 2.0f, y, length / 2.0f, depth - 1, params);

    // верхний
    draw_sierpinski_triangle(x + length / 4.0f, y - height / 2.0f, length / 2.0f, depth - 1, params);
}