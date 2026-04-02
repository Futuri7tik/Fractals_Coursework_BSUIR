#ifndef KURSACH_LIBRARY_H
#define KURSACH_LIBRARY_H

#include <SFML/Graphics.h>
#include <stdio.h>

#define MAX_VERTICES 10000000
extern sfVertex vertices[MAX_VERTICES];
extern int vertex_count;

typedef struct {
    int depth;
    int start_depth;
    double angle;
    double length_factor;
} TreeParameters;

typedef struct {
    int depth;
    int max_depth;
    double start_length;
} CarpetParameters;

typedef struct {
    int depth;
    int max_depth;
    double start_length;
} TriangleParameters;

void flush_fractal(sfRenderWindow* window, sfPrimitiveType primitive);

sfColor get_color_tree(int depth, const TreeParameters* parameters);

void add_line(double x_start, double y_start, double x_end, double y_end, sfColor color);

void draw_tree(sfRenderWindow* window, double x_start,
    double y_start, double length, double angle, int depth,
    TreeParameters* parameters);

int render_tree(void);

void add_square( double x, double y, double length, sfColor color);

sfColor get_color_carpet(int depth, CarpetParameters* params);

void draw_carpet (sfRenderWindow* window, double x_left, double y_left, double length, int depth, CarpetParameters* carpet);

int render_carpet(void);

void add_base(double x, double y, double length, sfColor color);

void add_triangle(double x, double y, double length, sfColor color);

sfColor get_color_triangle(int depth, TriangleParameters* params);

void draw_sierpinski_triangle(sfRenderWindow* window, double x, double y,
    double length, int depth, TriangleParameters* params);

int render_triangle(void);

#endif // KURSACH_LIBRARY_H