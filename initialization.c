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

    fscanf(f, "%f %d %f %f", &params->depth, &params->max_depth,
        &params->angle_degrees, &params->length_factor);
    params->angle = params->angle_degrees * DEG2RAD;

    fclose(f);
}

void init_default_tree_parameters(TreeParameters* params) {
    FILE *f = fopen("tree_default.txt", "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening tree_default.txt file\n");
        return;
    }

    fscanf(f, "%f %d %f %f", &params->depth, &params->max_depth, &params->angle_degrees, &params->length_factor);
    params->angle = params->angle_degrees * DEG2RAD;

    fclose(f);
}

void init_carpet_parameters(CarpetParameters* params) {
    FILE *f = fopen("carpet.txt", "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening carpet.txt file\n");
        return;
    }

    fscanf(f, "%f %d %f %f %f %f", &params->depth, &params->max_depth,
        &params->start_length, &params->red, &params->green, &params->blue);

    fclose(f);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_default_carpet_parameters(CarpetParameters* params) {
    FILE *f = fopen("carpet_default.txt", "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening carpet_default.txt file\n");
        return;
    }

    fscanf(f, "%f %d %f %f %f %f", &params->depth, &params->max_depth, &params->start_length, &params->red, &params->green, &params->blue);

    fclose(f);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_triangle_parameters(TriangleParameters* params) {
    FILE *f = fopen("triangle.txt", "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening triangle.txt file\n");
        return;
    }

    fscanf(f, "%f %d %f %f %f %f", &params->depth, &params->max_depth, &params->start_length,
        &params->red, &params->green, &params->blue);

    params->x_start = (WIDTH - params->start_length) / 2.0f;
    params->y_start = HEIGHT - (HEIGHT - params->start_length * sqrtf(3.0f) / 2.0f) / 2.0f;

    fclose(f);
}

void init_default_triangle_parameters(TriangleParameters* params) {
    FILE *f = fopen("triangle_default.txt", "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening triangle_default.txt file\n");
        return;
    }

    fscanf(f, "%f %d %f %f %f %f", &params->depth, &params->max_depth, &params->start_length,
        &params->red, &params->green, &params->blue);

    params->x_start = (WIDTH - params->start_length) / 2.0f;
    params->y_start = HEIGHT - (HEIGHT - params->start_length * sqrtf(3.0f) / 2.0f) / 2.0f;

    fclose(f);
}

void init_mandelbrot_parameters(MandelbrotParameters* params) {
    FILE *f = fopen("mandelbrot.txt", "r");
    if (f == NULL) {
        fprintf(stderr, "Error opening mandelbrot.txt file\n");
        return;
    }

    fscanf(f, "%f %d %f %f %f %f %f %f", &params->iterations, &params->max_iterations, &params->zoom,
        &params->offset_x, &params->offset_y, &params->red, &params->green, &params->blue);

    fclose(f);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_default_mandelbrot_parameters(MandelbrotParameters* params) {
    FILE *f = fopen("mandelbrot_default.txt", "r");
    if (f == NULL) {
        fprintf(stderr, "Error opening mandelbrot_default.txt file\n");
        return;
    }

    fscanf(f, "%f %d %f %f %f %f %f %f", &params->iterations, &params->max_iterations, &params->zoom,
        &params->offset_x, &params->offset_y, &params->red, &params->green, &params->blue);

    fclose(f);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_julia_parameters(JuliaParameters* params) {
    FILE *f = fopen("julia.txt", "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening julia.txt file\n");
        return;
    }

    fscanf(f, "%f %d %f %f %f %f %f %f %f %f", &params->iterations, &params->max_iterations, &params->zoom,
        &params->offset_x, &params->offset_y, &params->re_c, &params->im_c, &params->red,
        &params->green, &params->blue);

    fclose(f);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_default_julia_parameters(JuliaParameters* params) {
    FILE *f = fopen("julia_default.txt", "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening julia_default.txt file\n");
        return;
    }

    fscanf(f, "%f %d %f %f %f %f %f %f %f %f", &params->iterations, &params->max_iterations, &params->zoom,
        &params->offset_x, &params->offset_y, &params->re_c, &params->im_c, &params->red,
        &params->green, &params->blue);

    fclose(f);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_circle_parameters(CircleParameters* params) {
    FILE *f = fopen("circle.txt", "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening circle.txt file\n");
        return;
    }

    float x_center, y_center;
    fscanf(f, "%f %d %f %f %f %f %f %f", &params->depth, &params->max_depth, &x_center, &y_center,
        &params->radius, &params->red, &params->green, &params->blue);

    params->center = (Vector2){x_center, y_center};

    fclose(f);
}

void init_default_circle_parameters(CircleParameters* params) {
    FILE *f = fopen("circle_default.txt", "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening circle_default.txt file\n");
        return;
    }

    float x_center, y_center;
    fscanf(f, "%f %d %f %f %f %f %f %f", &params->depth, &params->max_depth, &x_center, &y_center,
        &params->radius, &params->red, &params->green, &params->blue);

    params->center = (Vector2){x_center, y_center};

    fclose(f);
}

void init_fern_parameters(FernParameters* params) {
    FILE *f = fopen("fern.txt", "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening fern.txt file\n");
        return;
    }

    fscanf(f, "%f %d %f %f %f %f", &params->iterations, &params->max_iterations, &params->prob1,
        &params->prob2, &params->prob3, &params->prob4);

    fclose(f);
    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_default_fern_parameters(FernParameters* params) {
    FILE *f = fopen("fern_default.txt", "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening fern_default.txt file\n");
        return;
    }

    fscanf(f, "%f %d %f %f %f %f", &params->iterations, &params->max_iterations, &params->prob1,
        &params->prob2, &params->prob3, &params->prob4);

    fclose(f);
}

void init_newton_parameters(NewtonParameters *params) {
    FILE *f = fopen("newton.txt", "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening newton.txt file\n");
        return;
    }

    fscanf(f, "%f %d %f %f %f %f %f %f %f %f %f", &params->iterations, &params->max_iterations, &params->zoom,
        &params->offset_x, &params->offset_y, &params->red, &params->green, &params->blue,
        &params->gradient_r, &params->gradient_g, &params->gradient_b);

    fclose(f);

    params->texture = LoadTextureFromImage(GenImageColor(WIDTH, HEIGHT, BLACK));
}

void init_default_newton_parameters(NewtonParameters* params) {
    FILE *f = fopen("newton_default.txt", "r");

    if (f == NULL) {
        fprintf(stderr, "Error opening newton_default.txt file\n");
        return;
    }

    fscanf(f, "%f %d %f %f %f %f %f %f %f %f %f", &params->iterations, &params->max_iterations, &params->zoom,
         &params->offset_x, &params->offset_y, &params->red, &params->green, &params->blue,
         &params->gradient_r, &params->gradient_g, &params->gradient_b);

    fclose(f);
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
}

void rewrite_tree_parameters(const TreeParameters* params) {
    FILE* f = fopen("tree.txt", "w");

    if (f == NULL) {
        fprintf(stderr, "Error opening tree.txt file\n");
    }

    fprintf(f, "%f\n%d\n%f\n%f\n", params->depth, params->max_depth, params->angle_degrees,
        params->length_factor);

    fclose(f);
}

void rewrite_carpet_parameters(const CarpetParameters* params) {
    FILE* f = fopen("carpet.txt", "w");

    if (f == NULL) {
        fprintf(stderr, "Error opening carpet.txt file\n");
    }

    fprintf(f, "%f\n%d\n%f\n%f\n%f\n%f\n", params->depth, params->max_depth, params->start_length,
        params->red, params->green, params->blue);
    fclose(f);
}

void rewrite_triangle_parameters(const TriangleParameters* params) {
    FILE* f = fopen("triangle.txt", "w");

    if (f == NULL) {
        fprintf(stderr, "Error opening carpet.txt file\n");
    }

    fprintf(f, "%f\n%d\n%f\n%f\n%f\n%f\n", params->depth, params->max_depth, params->start_length,
        params->red, params->green, params->blue);
    fclose(f);
}

void rewrite_mandelbrot_parameters(const MandelbrotParameters* params) {
    FILE* f = fopen("mandelbrot.txt", "w");

    if (f == NULL) {
        fprintf(stderr, "Error opening mandelbrot.txt file\n");
    }

    fprintf(f, "%f\n%d\n%f\n%f\n%f\n%f\n%f\n%f\n", params->iterations, params->max_iterations, params->zoom,
        params->offset_x, params->offset_y, params->red, params->green, params->blue);
    fclose(f);
}

void rewrite_julia_parameters(const JuliaParameters* params) {
    FILE* f = fopen("julia.txt", "w");

    if (f == NULL) {
        fprintf(stderr, "Error opening julia.txt file\n");
    }

    fprintf(f, "%f\n%d\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n", params->iterations, params->max_iterations,
        params->zoom, params->offset_x, params->offset_y, params->re_c, params->im_c, params->red, params->green, params->blue);

    fclose(f);
}

void rewrite_circle_parameters(const CircleParameters* params) {
    FILE* f = fopen("circle.txt", "w");

    if (f == NULL) {
        fprintf(stderr, "Error opening circle.txt file\n");
    }

    fprintf(f, "%f\n%d\n%f\n%f\n%f\n%f\n%f\n%f\n", params->depth, params->max_depth, params->center.x,
        params->center.y, params->radius, params->red, params->green, params->blue);

    fclose(f);
}

void rewrite_fern_parameters(const FernParameters* params) {
    FILE* f = fopen("fern.txt", "w");

    if (f == NULL) {
        fprintf(stderr, "Error opening fern.txt file\n");
    }

    fprintf(f, "%f\n%d\n%f\n%f\n%f\n%f\n", params->iterations, params->max_iterations, params->prob1,
        params->prob2, params->prob3, params->prob4);

    fclose(f);
}

void rewrite_newton_parameters(const NewtonParameters* params) {
    FILE* f = fopen("newton.txt", "w");

    if (f == NULL) {
        fprintf(stderr, "Error opening newton.txt file\n");
    }

    fprintf(f, "%f\n%d\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n%f\n", params->iterations, params->max_iterations,
        params->zoom, params->offset_x, params->offset_y, params->red, params->green, params->blue, params->gradient_r,
        params->gradient_g, params->gradient_b);

    fclose(f);
}