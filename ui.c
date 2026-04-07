#include "functions.h"

void init_tree_parameters(TreeParameters* params) {
    params->depth = 14;
    params->max_depth = 18;
    params->angle = 25.0f * DEG2RAD;
    params->angle_degrees = 25.0f;
    params->length_factor = 0.75f;
}

void init_carpet_parameters(CarpetParameters* params) {
    params->depth = 7;
    params->max_depth = 8;
    params->start_length = HEIGHT - 200;
    params->texture = (Texture2D) {0};
}

void init_triangle_parameters(TriangleParameters* params) {
    params->depth = 9;
    params->max_depth = 13;
    params->start_length = HEIGHT - 50;
}

void init_mandelbrot_parameters(MandelbrotParameters* params) {
    params->iterations = 200;
    params->max_iterations = 800;
    params->zoom = 1.0f;
    params->offset_x = 0.0f;
    params->offset_y = 0.0f;
    params->red = 9;
    params->green = 15;
    params->blue = 8.5f;
    params->texture = (Texture2D) {0};
}