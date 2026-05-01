#include "raylib.h"
#include "functions.h"

static int mandelbrot_iterations(const float re_c, const float im_c, const int max_iterations) {
    const float q = (re_c - 0.25f) * (re_c - 0.25f) + im_c * im_c;
    if (q * (q + (re_c - 0.25f)) < 0.25f * im_c * im_c) {
        return max_iterations;
    }

    if ((re_c + 1.0f) * (re_c + 1.0f) + im_c * im_c < 0.0625f) {
        return max_iterations;
    }

    int iterations = 0;
    float a = 0.0f, b = 0.0f;
    float a2 = 0.0f, b2 = 0.0f;
    float ab = a * b;

    while (a2 + b2 < 4.0f && iterations < max_iterations) {
        b = 2 * ab + im_c;
        a = a2 - b2 + re_c;

        a2 = a * a;
        b2 = b * b;
        ab = a * b;
        iterations++;
    }

    return iterations;
}

static Color get_color_mandelbrot(const int iteration, const int max_iterations, const MandelbrotParameters* params) {
    if (iteration == max_iterations) {
        return BLACK;
    }

    const float t = (float)iteration / (float)max_iterations;
    unsigned char r = (unsigned char)(params->red * (1-t) * t * t * t * 255);
    unsigned char g = (unsigned char)(params->green * (1-t) * (1-t) * t * t * 255);
    unsigned char b = (unsigned char)(params->blue * (1-t) * (1-t) * (1-t) * t * 255);

    return (Color) {r, g, b, 255};
}

void render_mandelbrot(float zoom, float offset_x,
                            float offset_y, const int max_iterations, const MandelbrotParameters *params) {
    static Color pixels[WIDTH * HEIGHT];

    #pragma omp parallel for schedule(dynamic) default(none) \
    shared(pixels, zoom, offset_x, offset_y, max_iterations, params)
    for (int y = 0; y < HEIGHT; ++y) {
        const float im_c = ((float)y - (float)HEIGHT / 2.0f) * (4.0f / ((float) WIDTH * zoom)) + offset_y;

        for (int x = 0; x < WIDTH; ++x) {
            const float re_c = ((float)x - (float)WIDTH / 2.0f) * (4.0f / ((float) WIDTH * zoom)) + offset_x;

            const int iterations = mandelbrot_iterations(re_c, im_c, max_iterations);
            pixels[y * WIDTH + x] = get_color_mandelbrot(iterations, max_iterations, params);
        }
    }

    UpdateTexture(params->texture, pixels);
}


int mandelbrot_fourth_iterations(const float re_c, const float im_c, const int max_iterations) {
    // const float q = (re_c - 0.25f) * (re_c - 0.25f) + im_c * im_c;
    // if (q * (q + (re_c - 0.25f)) < 0.25f * im_c * im_c) {
    //     return max_iterations;
    // }
    //
    // if ((re_c + 1.0f) * (re_c + 1.0f) + im_c * im_c < 0.0625f) {
    //     return max_iterations;
    // }

    int iterations = 0;
    float a = 0.0f, b = 0.0f;
    float a2 = 0.0f, b2 = 0.0f, ab = 0.0f;

    while (a2 + b2 < 4.0f && iterations < max_iterations) {
        b = 4 * (a2 * ab - b2 * ab) + im_c;
        a = a2 * a2 - 6 * ab * ab + b2 * b2 + re_c;

        a2 = a * a;
        b2 = b * b;
        ab = a * b;
        iterations++;
    }

    return iterations;
}