#include "raylib.h"
#include "functions.h"

static int julia_iterations(float re_z, float im_z, const float re_c, const float im_c, const int max_iterations) {
    int iterations = 0;
    float re_z2 = re_z * re_z;
    float im_z2 = im_z * im_z;
    float prod = re_z * im_z;

    while (re_z2 + im_z2 < 4.0f && iterations < max_iterations) {
        im_z = prod + prod + im_c;
        re_z = re_z2 - im_z2 + re_c;

        re_z2 = re_z * re_z;
        im_z2 = im_z * im_z;
        prod = re_z * im_z;
        iterations++;
    }

    return iterations;
}

static Color get_color_julia(const int iteration, const int max_iterations, const JuliaParameters* params) {
    if (iteration == max_iterations) {
        return BLACK;
    }

    const float t = (float)iteration / (float)max_iterations;
    unsigned char r = (unsigned char)(params->red * (1-t) * t * t * t * 255);
    unsigned char g = (unsigned char)(params->green * (1-t) * (1-t) * t * t * 255);
    unsigned char b = (unsigned char)(params->blue * (1-t) * (1-t) * (1-t) * t * 255);

    return (Color) {r, g, b, 255};
}

void render_julia(float zoom, float offset_x,
    float offset_y, const int max_iterations, const JuliaParameters* params) {
    static Color pixels[WIDTH * HEIGHT];

    #pragma omp parallel for schedule(dynamic) default(none) \
    shared(pixels, zoom, offset_x, offset_y, max_iterations, params)
    for (int y = 0; y < HEIGHT; y++) {
        const float im_z = ((float)y - (float)HEIGHT / 2.0f) * (4.0f / ((float) WIDTH * zoom)) + offset_y;
        
        for (int x = 0; x < WIDTH; x++) {
            const float re_z = ((float)x - (float)WIDTH / 2.0f) * (4.0f / ((float) WIDTH * zoom)) + offset_x;

            const int iterations = julia_iterations(re_z, im_z, params->re_c, params->im_c, max_iterations);
            pixels[y * WIDTH + x] = get_color_julia(iterations, max_iterations, params);
        }
    }

    UpdateTexture(params->texture, pixels);
}