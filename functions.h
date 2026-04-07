#ifndef KURSACH_FUNCTIONS_H
#define KURSACH_FUNCTIONS_H
#include "raylib.h"
#define WIDTH 1920
#define HEIGHT 1080

typedef enum {
    STATE_MENU,
    STATE_GALLERY,
    STATE_TREE,
    STATE_CARPET,
    STATE_TRIANGLE,
    STATE_MANDELBROT
} AppState;

typedef struct {
    float min;
    float max;
    float value;
} SliderParameters;

// === Параметры фракталов ===
typedef struct {
    float depth;
    int max_depth;
    float angle;
    float angle_degrees;
    float length_factor;
} TreeParameters;

typedef struct {
    float depth;
    int max_depth;
    float start_length;
    Texture2D texture;
} CarpetParameters;

typedef struct {
    float depth;
    int max_depth;
    float start_length;
} TriangleParameters;

typedef struct {
    float iterations;
    int max_iterations;
    float zoom;
    float offset_x;
    float offset_y;
    float red;
    float green;
    float blue;
    Texture2D texture;
} MandelbrotParameters;


// === UI ===
void init_tree_parameters(TreeParameters* params);
void init_carpet_parameters(CarpetParameters* params);

// === Дерево ===
Color get_color_tree(int depth, const TreeParameters* params);
void draw_tree(float x_start, float y_start, float length, float angle,
               int depth, TreeParameters* params);

// === Ковёр ===
void draw_square(float x, float y, float length, Color color);
void draw_carpet(float x_left, float y_left, float length,
                 int depth, CarpetParameters* params);
void draw_carpet_to_image(Image* img, float x, float y, float length, int depth);
Texture2D render_carpet_to_texture(int width, int height, int depth, float start_length);

// === Треугольник ===
Color get_color_triangle(int depth, TriangleParameters* params);
void draw_triangle_base(float x, float y, float length, Color color);
void draw_center_triangle(float x, float y, float length, Color color);
void draw_sierpinski_triangle(float x, float y, float length,
                              int depth, TriangleParameters* params);

// === Множество мандельброта
int mandelbrot_iterations(float re_c, float im_c, int max_iterations);
Color get_color_mandelbrot(int iteration, int max_iterations, const MandelbrotParameters* params);
Texture2D render_mandelbrot(int width, int height, float zoom, float offset_x,
    float offset_y, int max_iterations, const MandelbrotParameters* params);
int mandelbrot_fourth_iterations(float re_c, float im_c, int max_iterations);

#endif