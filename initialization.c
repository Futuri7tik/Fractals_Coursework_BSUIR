#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include "functions.h"
#include <stdarg.h>

static void load_params(const char* filename, const char* format, ...) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Error opening %s\n", filename);
        return;
    }
    va_list args;
    va_start(args, format);
    vfscanf(f, format, args);
    va_end(args);
    fclose(f);
}

static void save_params(const char* filename, const char* format, ...) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Error opening %s\n", filename);
        return;
    }
    va_list args;
    va_start(args, format);
    vfprintf(f, format, args);
    va_end(args);
    fclose(f);
}

void init_fractals_parameters(FractalParameters* params) {
    init_tree_parameters(&params->tree);
    init_carpet_parameters(&params->carpet);
    init_triangle_parameters(&params->triangle);
    init_mandelbrot_parameters(&params->mandelbrot);
    init_julia_parameters(&params->julia);
    init_circle_parameters(&params->circle);
    init_fern_parameters(&params->fern);
    init_newton_parameters(&params->newton);
}

void init_tree_parameters(TreeParameters* params) {
    load_params("tree.txt", "%f %d %f %f", &params->depth, &params->max_depth, &params->angle_degrees, &params->length_factor);
    params->angle = params->angle_degrees * DEG2RAD;
}

void init_default_tree_parameters(TreeParameters* params) {
    load_params("tree_default.txt", "%f %d %f %f", &params->depth, &params->max_depth, &params->angle_degrees, &params->length_factor);
    params->angle = params->angle_degrees * DEG2RAD;
}

void init_carpet_parameters(CarpetParameters* params) {
    load_params("carpet.txt", "%f %d %f %f %f %f", &params->depth, &params->max_depth, &params->start_length, &params->red, &params->green, &params->blue);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_default_carpet_parameters(CarpetParameters* params) {
    load_params("carpet_default.txt", "%f %d %f %f %f %f", &params->depth, &params->max_depth, &params->start_length, &params->red, &params->green, &params->blue);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_triangle_parameters(TriangleParameters* params) {
    load_params("triangle.txt", "%f %d %f %f %f %f", &params->depth, &params->max_depth, &params->start_length, &params->red, &params->green, &params->blue);
    params->x_start = (WIDTH - params->start_length) / 2.0f;
    params->y_start = HEIGHT - (HEIGHT - params->start_length * sqrtf(3.0f) / 2.0f) / 2.0f;
}

void init_default_triangle_parameters(TriangleParameters* params) {
    load_params("triangle_default.txt", "%f %d %f %f %f %f", &params->depth, &params->max_depth, &params->start_length, &params->red, &params->green, &params->blue);
    params->x_start = (WIDTH - params->start_length) / 2.0f;
    params->y_start = HEIGHT - (HEIGHT - params->start_length * sqrtf(3.0f) / 2.0f) / 2.0f;
}

void init_mandelbrot_parameters(MandelbrotParameters* params) {
    load_params("mandelbrot.txt", "%f %d %f %f %f %f %f %f", &params->iterations, &params->max_iterations, &params->zoom, &params->offset_x, &params->offset_y, &params->red, &params->green, &params->blue);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_default_mandelbrot_parameters(MandelbrotParameters* params) {
    load_params("mandelbrot_default.txt", "%f %d %f %f %f %f %f %f", &params->iterations, &params->max_iterations, &params->zoom, &params->offset_x, &params->offset_y, &params->red, &params->green, &params->blue);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_julia_parameters(JuliaParameters* params) {
    load_params("julia.txt", "%f %d %f %f %f %f %f %f %f %f", &params->iterations, &params->max_iterations, &params->zoom, &params->offset_x, &params->offset_y, &params->re_c, &params->im_c, &params->red, &params->green, &params->blue);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_default_julia_parameters(JuliaParameters* params) {
    load_params("julia_default.txt", "%f %d %f %f %f %f %f %f %f %f", &params->iterations, &params->max_iterations, &params->zoom, &params->offset_x, &params->offset_y, &params->re_c, &params->im_c, &params->red, &params->green, &params->blue);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_circle_parameters(CircleParameters* params) {
    float x_center, y_center;
    load_params("circle.txt", "%f %d %f %f %f %f %f %f", &params->depth, &params->max_depth, &x_center, &y_center, &params->radius, &params->red, &params->green, &params->blue);
    params->center = (Vector2){x_center, y_center};
}

void init_default_circle_parameters(CircleParameters* params) {
    float x_center, y_center;
    load_params("circle_default.txt", "%f %d %f %f %f %f %f %f", &params->depth, &params->max_depth, &x_center, &y_center, &params->radius, &params->red, &params->green, &params->blue);
    params->center = (Vector2){x_center, y_center};
}

void init_fern_parameters(FernParameters* params) {
    load_params("fern.txt", "%f %d %f %f %f %f", &params->iterations, &params->max_iterations, &params->prob1, &params->prob2, &params->prob3, &params->prob4);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_default_fern_parameters(FernParameters* params) {
    load_params("fern_default.txt", "%f %d %f %f %f %f", &params->iterations, &params->max_iterations, &params->prob1, &params->prob2, &params->prob3, &params->prob4);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_newton_parameters(NewtonParameters *params) {
    load_params("newton.txt", "%f %d %f %f %f %f %f %f %f %f %f", &params->iterations, &params->max_iterations, &params->zoom, &params->offset_x, &params->offset_y, &params->red, &params->green, &params->blue, &params->gradient_r, &params->gradient_g, &params->gradient_b);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_default_newton_parameters(NewtonParameters* params) {
    load_params("newton_default.txt", "%f %d %f %f %f %f %f %f %f %f %f", &params->iterations, &params->max_iterations, &params->zoom, &params->offset_x, &params->offset_y, &params->red, &params->green, &params->blue, &params->gradient_r, &params->gradient_g, &params->gradient_b);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

// === Сохранение параметров ===

void rewrite_fractal_parameters(const FractalParameters* params) {
    rewrite_tree_parameters(&params->tree);
    rewrite_carpet_parameters(&params->carpet);
    rewrite_triangle_parameters(&params->triangle);
    rewrite_mandelbrot_parameters(&params->mandelbrot);
    rewrite_julia_parameters(&params->julia);
    rewrite_circle_parameters(&params->circle);
    rewrite_fern_parameters(&params->fern);
    rewrite_newton_parameters(&params->newton);
}

void rewrite_tree_parameters(const TreeParameters* params) {
    save_params("tree.txt", "%f\n%d\n%f\n%f\n", params->depth, params->max_depth, params->angle_degrees, params->length_factor);
}

void rewrite_carpet_parameters(const CarpetParameters* params) {
    save_params("carpet.txt", "%f\n%d\n%f\n%f\n%f\n%f\n", params->depth, params->max_depth, params->start_length, params->red, params->green, params->blue);
}

void rewrite_triangle_parameters(const TriangleParameters* params) {
    save_params("triangle.txt", "%f\n%d\n%f\n%f\n%f\n%f\n", params->depth, params->max_depth, params->start_length, params->red, params->green, params->blue);
}

void rewrite_mandelbrot_parameters(const MandelbrotParameters* params) {
    save_params("mandelbrot.txt", "%f\n%d\n%f\n%f\n%f\n%f\n%f\n%f\n", params->iterations, params->max_iterations, params->zoom, params->offset_x, params->offset_y, params->red, params->green, params->blue);
}

void rewrite_julia_parameters(const JuliaParameters* params) {
    save_params("julia.txt", "%f\n%d\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n", params->iterations, params->max_iterations, params->zoom, params->offset_x, params->offset_y, params->re_c, params->im_c, params->red, params->green, params->blue);
}

void rewrite_circle_parameters(const CircleParameters* params) {
    save_params("circle.txt", "%f\n%d\n%f\n%f\n%f\n%f\n%f\n%f\n", params->depth, params->max_depth, params->center.x, params->center.y, params->radius, params->red, params->green, params->blue);
}

void rewrite_fern_parameters(const FernParameters* params) {
    save_params("fern.txt", "%f\n%d\n%f\n%f\n%f\n%f\n", params->iterations, params->max_iterations, params->prob1, params->prob2, params->prob3, params->prob4);
}

void rewrite_newton_parameters(const NewtonParameters* params) {
    save_params("newton.txt", "%f\n%d\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n", params->iterations, params->max_iterations, params->zoom, params->offset_x, params->offset_y, params->red, params->green, params->blue, params->gradient_r, params->gradient_g, params->gradient_b);
}