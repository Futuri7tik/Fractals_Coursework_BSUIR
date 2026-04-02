#ifndef KURSACH_FUNCTIONS_H
#define KURSACH_FUNCTIONS_H

#include "raylib.h"
#include "raymath.h"

// === Параметры фракталов ===
typedef struct {
    int depth;
    int max_depth;
    int start_depth;
    float angle;
    float length_factor;
} TreeParameters;

typedef struct {
    int depth;
    int max_depth;
    float start_length;
} CarpetParameters;

typedef struct {
    int depth;
    int max_depth;
    float start_length;
} TriangleParameters;

typedef struct {
    int iterations;
    int max_iterations;
    float zoom;
    float offset_x;
    float offset_y;
} MandelbrotParameters;

// === Дерево ===
Color get_color_tree(int depth, const TreeParameters* params);
void draw_tree(float x_start, float y_start, float length, float angle,
               int depth, TreeParameters* params);

// === Ковёр ===
Color get_color_carpet(int depth, CarpetParameters* params);
void draw_square(float x, float y, float length, Color color);
void draw_carpet(float x_left, float y_left, float length,
                 int depth, CarpetParameters* params);

// === Треугольник ===
Color get_color_triangle(int depth, TriangleParameters* params);
void draw_triangle_base(float x, float y, float length, Color color);
void draw_center_triangle(float x, float y, float length, Color color);  // ← Добавить!
void draw_sierpinski_triangle(float x, float y, float length,
                              int depth, TriangleParameters* params);

#endif