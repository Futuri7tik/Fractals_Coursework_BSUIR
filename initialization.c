#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include "functions.h"

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
    FILE *f = fopen("tree.txt", "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening tree.txt file\n");
        return;
    }

    fscanf(f, "%f", &params->depth);
    fscanf(f, "%d", &params->max_depth);
    fscanf(f, "%f", &params->angle_degrees);
    params->angle = params->angle_degrees * DEG2RAD;
    fscanf(f, "%f", &params->length_factor);

    fclose(f);
}

void init_carpet_parameters(CarpetParameters* params) {
    FILE *f = fopen("carpet.txt", "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening carpet.txt file\n");
        return;
    }

    fscanf(f, "%f", &params->depth);
    fscanf(f, "%d", &params->max_depth);
    fscanf(f, "%f", &params->start_length);
    fscanf(f, "%f", &params->red);
    fscanf(f, "%f", &params->green);
    fscanf(f, "%f", &params->blue);

    fclose(f);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_triangle_parameters(TriangleParameters* params) {
    FILE *f = fopen("triangle.txt", "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening triangle.txt file\n");
        return;
    }

    fscanf(f, "%f", &params->depth);
    fscanf(f, "%d", &params->max_depth);
    fscanf(f, "%f", &params->start_length);

    params->x_start = (1920 - params->start_length) / 2.0f;
    params->y_start = 1080 - (1080 - params->start_length * sqrtf(3.0f) / 2.0f) / 2.0f;

    fscanf(f, "%f", &params->red);
    fscanf(f, "%f", &params->green);
    fscanf(f, "%f", &params->blue);

    fclose(f);
}

void init_mandelbrot_parameters(MandelbrotParameters* params) {
    FILE *f = fopen("mandelbrot.txt", "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening mandelbrot.txt file\n");
        return;
    }

    fscanf(f, "%f", &params->iterations);
    fscanf(f, "%d", &params->max_iterations);
    fscanf(f, "%f", &params->zoom);
    fscanf(f, "%f", &params->offset_x);
    fscanf(f, "%f", &params->offset_y);
    fscanf(f, "%f", &params->red);
    fscanf(f, "%f", &params->green);
    fscanf(f, "%f", &params->blue);

    fclose(f);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_julia_parameters(JuliaParameters* params) {
    FILE *f = fopen("julia.txt", "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening julia.txt file\n");
        return;
    }

    fscanf(f, "%f", &params->iterations);
    fscanf(f, "%d", &params->max_iterations);
    fscanf(f, "%f", &params->zoom);
    fscanf(f, "%f", &params->offset_x);
    fscanf(f, "%f", &params->offset_y);
    fscanf(f, "%f", &params->re_c);
    fscanf(f, "%f", &params->im_c);
    fscanf(f, "%f", &params->red);
    fscanf(f, "%f", &params->green);
    fscanf(f, "%f", &params->blue);

    fclose(f);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_circle_parameters(CircleParameters* params) {
    FILE *f = fopen("circle.txt", "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening circle.txt file\n");
        return;
    }

    fscanf(f, "%f", &params->depth);
    fscanf(f, "%d", &params->max_depth);

    float x_center, y_center;
    fscanf(f, "%f", &x_center);
    fscanf(f, "%f", &y_center);
    params->center = (Vector2){x_center, y_center};

    fscanf(f, "%f", &params->radius);
    fscanf(f, "%f", &params->red);
    fscanf(f, "%f", &params->green);
    fscanf(f, "%f", &params->blue);

    fclose(f);
}

void init_fern_parameters(FernParameters* params) {
    FILE *f = fopen("fern.txt", "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening fern.txt file\n");
        return;
    }

    fscanf(f, "%f", &params->iterations);
    fscanf(f, "%d", &params->max_iterations);

    fscanf(f, "%f", &params->prob1);
    fscanf(f, "%f", &params->prob2);
    fscanf(f, "%f", &params->prob3);
    fscanf(f, "%f", &params->prob4);

    fclose(f);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_newton_parameters(NewtonParameters *params) {
    FILE *f = fopen("newton.txt", "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening newton.txt file\n");
        return;
    }

    fscanf(f, "%f", &params->iterations);
    fscanf(f, "%d", &params->max_iterations);
    fscanf(f, "%f", &params->zoom);
    fscanf(f, "%f", &params->offset_x);
    fscanf(f, "%f", &params->offset_y);
    fscanf(f, "%f", &params->red);
    fscanf(f, "%f", &params->green);
    fscanf(f, "%f", &params->blue);
    fscanf(f, "%f", &params->gradient_r);
    fscanf(f, "%f", &params->gradient_g);
    fscanf(f, "%f", &params->gradient_b);

    fclose(f);

    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}