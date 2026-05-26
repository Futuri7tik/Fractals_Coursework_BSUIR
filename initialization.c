#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include "functions.h"

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
    init_tree_parameters(&params->tree, false);
    init_carpet_parameters(&params->carpet, false);
    init_triangle_parameters(&params->triangle, false);
    init_mandelbrot_parameters(&params->mandelbrot, false);
    init_julia_parameters(&params->julia, false);
    init_circle_parameters(&params->circle, false);
    init_fern_parameters(&params->fern, false);
    init_newton_parameters(&params->newton, false);
    init_dragon_parameters(&params->dragon, false);
    init_polynom_mandel_parameters(&params->polynom_mandel, false);
    init_lyapunov_parameters(&params->lyapunov, false);
}

void init_tree_parameters(TreeParameters* params, const bool is_default) {
    const char *filename = is_default ? "tree_default.txt" : "tree.txt";

    load_params(filename, "%f %d %f %f", &params->depth, &params->max_depth, &params->angle_degrees, &params->length_factor);
    params->angle = params->angle_degrees * DEG2RAD;
}

void init_carpet_parameters(CarpetParameters* params, const bool is_default) {
    const char *filename = is_default ? "carpet_default.txt" : "carpet.txt";

    load_params(filename, "%f %d %f %f %f %f", &params->depth, &params->max_depth, &params->start_length, &params->red, &params->green, &params->blue);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_triangle_parameters(TriangleParameters* params, const bool is_default) {
    const char *filename = is_default ? "triangle_default.txt" : "triangle.txt";

    load_params(filename, "%f %d %f %f %f %f", &params->depth, &params->max_depth, &params->start_length, &params->red, &params->green, &params->blue);
    params->x_start = (WIDTH - params->start_length) / 2.0f;
    params->y_start = HEIGHT - (HEIGHT - params->start_length * sqrtf(3.0f) / 2.0f) / 2.0f;
}

void init_mandelbrot_parameters(MandelbrotParameters* params, const bool is_default) {
    const char *filename = is_default ? "mandelbrot_default.txt" : "mandelbrot.txt";

    load_params(filename, "%f %d %f %f %f %f %f %f", &params->iterations, &params->max_iterations, &params->zoom, &params->offset_x, &params->offset_y, &params->red, &params->green, &params->blue);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_julia_parameters(JuliaParameters* params, const bool is_default) {
    const char *filename = is_default ? "julia_default.txt" : "julia.txt";

    load_params(filename, "%f %d %f %f %f %f %f %f %f %f", &params->iterations, &params->max_iterations, &params->zoom, &params->offset_x, &params->offset_y, &params->re_c, &params->im_c, &params->red, &params->green, &params->blue);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_circle_parameters(CircleParameters* params, const bool is_default) {
    const char *filename = is_default ? "circle_default.txt" : "circle.txt";
    float x_center, y_center;

    load_params(filename, "%f %d %f %f %f %f %f %f", &params->depth, &params->max_depth, &x_center, &y_center, &params->radius, &params->red, &params->green, &params->blue);
    params->center = (Vector2){x_center, y_center};
}

void init_fern_parameters(FernParameters* params, const bool is_default) {
    const char *filename = is_default ? "fern_default.txt" : "fern.txt";

    load_params(filename, "%f %d %f %f %f %f", &params->iterations, &params->max_iterations, &params->prob1, &params->prob2, &params->prob3, &params->prob4);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_newton_parameters(NewtonParameters *params, const bool is_default) {
    const char *filename = is_default ? "newton_default.txt" : "newton.txt";

    load_params(filename, "%f %d %f %f %f %f %f %f %f %f %f", &params->iterations, &params->max_iterations, &params->zoom, &params->offset_x, &params->offset_y, &params->red, &params->green, &params->blue, &params->gradient_r, &params->gradient_g, &params->gradient_b);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_dragon_parameters(DragonParameters* params, const bool is_default) {
    const char *filename = is_default ? "dragon_default.txt" : "dragon.txt";

    load_params(filename, "%f %d %f %f %f %f %f", &params->depth, &params->max_depth, &params->angle_tilt,
        &params->length, &params->red, &params->green, &params->blue);

    params->x = WIDTH / 2.0f;
    params->y = HEIGHT / 2.0f;
    params->angle = 0.0f;
}

void init_polynom_mandel_parameters(PolynomMandelParameters* params, const bool is_default) {
    const char *filename = is_default ? "polynomial_mandelbrot_default.txt" : "polynomial_mandelbrot.txt";

    load_params(filename, "%f %d %f %f %f %f %f %f %f",
        &params->iterations, &params->max_iterations, &params->zoom, &params->offset_x, &params->offset_y,
        &params->red, &params->green, &params->blue, &params->alpha);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_lyapunov_parameters(LyapunovParameters* params, const bool is_default)  {
    const char *filename = is_default ? "lyapunov_default.txt" : "lyapunov.txt";
    const int SIZE = 15;
    params->sequence = (char*)malloc((SIZE + 1) * sizeof(char));

    load_params(filename, "%d %f %15s %f %f %f", &params->max_iterations, &params->iterations,
        params->sequence, &params->red, &params->green, &params->blue);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

static void rewrite_tree_parameters(const TreeParameters* params) {
    save_params("tree.txt", "%f\n%d\n%f\n%f\n", params->depth, params->max_depth, params->angle_degrees, params->length_factor);
}

static void rewrite_carpet_parameters(const CarpetParameters* params) {
    save_params("carpet.txt", "%f\n%d\n%f\n%f\n%f\n%f\n", params->depth, params->max_depth, params->start_length, params->red, params->green, params->blue);
}

static void rewrite_triangle_parameters(const TriangleParameters* params) {
    save_params("triangle.txt", "%f\n%d\n%f\n%f\n%f\n%f\n", params->depth, params->max_depth, params->start_length, params->red, params->green, params->blue);
}

static void rewrite_mandelbrot_parameters(const MandelbrotParameters* params) {
    save_params("mandelbrot.txt", "%f\n%d\n%f\n%f\n%f\n%f\n%f\n%f\n", params->iterations, params->max_iterations, params->zoom, params->offset_x, params->offset_y, params->red, params->green, params->blue);
}

static void rewrite_julia_parameters(const JuliaParameters* params) {
    save_params("julia.txt", "%f\n%d\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n", params->iterations, params->max_iterations, params->zoom, params->offset_x, params->offset_y, params->re_c, params->im_c, params->red, params->green, params->blue);
}

static void rewrite_circle_parameters(const CircleParameters* params) {
    save_params("circle.txt", "%f\n%d\n%f\n%f\n%f\n%f\n%f\n%f\n", params->depth, params->max_depth, params->center.x, params->center.y, params->radius, params->red, params->green, params->blue);
}

static void rewrite_fern_parameters(const FernParameters* params) {
    save_params("fern.txt", "%f\n%d\n%f\n%f\n%f\n%f\n", params->iterations, params->max_iterations, params->prob1, params->prob2, params->prob3, params->prob4);
}

static void rewrite_newton_parameters(const NewtonParameters* params) {
    save_params("newton.txt", "%f\n%d\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n", params->iterations, params->max_iterations, params->zoom, params->offset_x, params->offset_y, params->red, params->green, params->blue, params->gradient_r, params->gradient_g, params->gradient_b);
}

static void rewrite_dragon_parameters(const DragonParameters* params) {
    save_params("dragon.txt", "%f\n%d\n%f\n%f\n%f\n%f\n%f\n", params->depth, params->max_depth, params->angle_tilt,
        params->length, params->red, params->green, params->blue);
}

static void rewrite_polynom_mandel_params(const PolynomMandelParameters* params) {
    save_params("polynomial_mandelbrot.txt", "%f\n%d\n%f\n%f\n%f\n%f\n%f\n%f\n%f",
        params->iterations, params->max_iterations, params->zoom, params->offset_x, params->offset_y,
        params->red, params->green, params->blue, params->alpha);
}

static void rewrite_lyapunov_parameters(const LyapunovParameters* params) {
    save_params("lyapunov.txt", "%d\n%f\n%15s\n%f\n%f\n%f", params->max_iterations,
        params->iterations, params->sequence, params->red, params->green, params->blue);
}

void rewrite_fractal_parameters(const FractalParameters* params) {
    rewrite_tree_parameters(&params->tree);
    rewrite_carpet_parameters(&params->carpet);
    rewrite_triangle_parameters(&params->triangle);
    rewrite_mandelbrot_parameters(&params->mandelbrot);
    rewrite_julia_parameters(&params->julia);
    rewrite_circle_parameters(&params->circle);
    rewrite_fern_parameters(&params->fern);
    rewrite_newton_parameters(&params->newton);
    rewrite_dragon_parameters(&params->dragon);
    rewrite_polynom_mandel_params(&params->polynom_mandel);
    rewrite_lyapunov_parameters(&params->lyapunov);
}
