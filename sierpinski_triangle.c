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
    if (depth <= 0) return;

    float height = length * SQRT3 / 2.0f;
    float h2 = height / 2.0f;
    float l2 = length / 2.0f;
    float l4 = length / 4.0f;


    DrawTriangle((Vector2){ x + l4, y - h2 },(Vector2){ x + l2, y },
    (Vector2){ x + 3.0f * l4, y - h2 },
        BLACK
    );

    // Левый нижний
    draw_sierpinski_triangle(x, y, l2, depth - 1, params);
    // Правый нижний
    draw_sierpinski_triangle(x + l2, y, l2, depth - 1, params);
    // Верхний
    draw_sierpinski_triangle(x + l4, y - h2, l2, depth - 1, params);
}