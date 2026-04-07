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