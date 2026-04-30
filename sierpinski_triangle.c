#include "raylib.h"
#include "functions.h"
#include "raymath.h"

void draw_triangle_base(float x, float y, float length, Color color) {
    float height = length * SQRT3 / 2.0f;
    DrawTriangle(
        (Vector2){x, y},                          // Левый нижний
        (Vector2){x + length, y},                 // Правый нижний
        (Vector2){x + length / 2.0f, y - height}, // Верхний
        color
    );
}

void draw_sierpinski_triangle(float x, float y, float length,
                              int depth, TriangleParameters* params) {
    if (depth <= 0)
        return;

    float height = length * SQRT3 / 2.0f;

    DrawTriangle((Vector2){x + length / 4.0f, y - height / 2.0f},(Vector2){ x + length / 2.0f, y },
    (Vector2){ x + 3.0f * length / 4.0f, y - height / 2.0f},
        BLACK
    );

    // Левый нижний
    draw_sierpinski_triangle(x, y, length / 2.0f, depth - 1, params);
    // Правый нижний
    draw_sierpinski_triangle(x + length / 2.0f, y, length / 2.0f, depth - 1, params);
    // Верхний
    draw_sierpinski_triangle(x + length / 4.0f, y - height / 2.0f, length / 2.0f, depth - 1, params);
}