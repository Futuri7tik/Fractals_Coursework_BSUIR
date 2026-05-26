#include <bemapiset.h>
#include <tgmath.h>
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
    float a = 0.0f, b = 0.0f, a2 = 0.0f, b2 = 0.0f, ab = a * b;

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

static int mandelbrot_polynomial_iterations(const float re_c, const float im_c, const int max_iterations,
    const float alph) {
    int iterations = 0;
    float a = 0.0f, b = 0.0f, a2 = 0.0f, b2 = 0.0f, ab;

    const float r = (2.0f > floorf(alph) + 1) ? 2.0f : floorf(a) + 1, r2 = r * r;

    while (a2 + b2 < r2 && iterations < max_iterations) {
        a2 = a * a, b2 = b * b;
        ab = a * b;

        float a_next = a * (a2 - 3 * b2) + alph * (a2 - b2) + re_c;
        float b_next = b * (3 * a2 - b2) + 2 * alph * ab + im_c;

        a = a_next;
        b = b_next;
        iterations++;
    }

    return iterations;
}

static Color get_color_mandelbrot(const int iteration, const int max_iterations, const MandelbrotParameters* params) {
    if (iteration == max_iterations) {
        return BLACK;
    }

    const float t = (float)iteration / (float)max_iterations;
    const unsigned char r = (unsigned char)(params->red * (1-t) * t * t * t * 255),
    g = (unsigned char)(params->green * (1-t) * (1-t) * t * t * 255),
    b = (unsigned char)(params->blue * (1-t) * (1-t) * (1-t) * t * 255);

    return (Color) {r, g, b, 255};
}

static Color get_color_polynom_mandelbrot(const int iteration, const int max_iterations, const PolynomMandelParameters* params) {
    if (iteration == max_iterations) {
        return BLACK;
    }

    const float t = (float)iteration / (float)max_iterations;
    const unsigned char r = (unsigned char)(params->red * (1-t) * t * t * t * 255),
    g = (unsigned char)(params->green * (1-t) * (1-t) * t * t * 255),
    b = (unsigned char)(params->blue * (1-t) * (1-t) * (1-t) * t * 255);

    return (Color) {r, g, b, 255};
}

void render_mandelbrot(float zoom, float offset_x,
                            float offset_y, const int max_iterations, const MandelbrotParameters *params) {
    static Color pixels[WIDTH * HEIGHT];
    const float scale_x = 4.0f / (WIDTH * zoom), center_x = WIDTH / 2.0f, center_y = HEIGHT / 2.0f;

    #pragma omp parallel for schedule(dynamic) default(none) \
    shared(pixels, offset_x, offset_y, max_iterations, params, center_y, center_x, scale_x)
    for (int y = 0; y < HEIGHT; ++y) {
        const float im_c = ((float)y - center_y) * scale_x + offset_y;

        for (int x = 0; x < WIDTH; ++x) {
            const float re_c = ((float)x - center_x) * scale_x + offset_x;

            const int iterations = mandelbrot_iterations(re_c, im_c, max_iterations);
            pixels[y * WIDTH + x] = get_color_mandelbrot(iterations, max_iterations, params);
        }
    }

    UpdateTexture(params->texture, pixels);
}

void render_polynom_mandel(float zoom, float offset_x, float offset_y,
    const int max_iterations, const PolynomMandelParameters *params) {
    static Color pixels[WIDTH * HEIGHT];
    const float scale_x = 4.0f / (WIDTH * zoom), center_x = WIDTH / 2.0f, center_y = HEIGHT / 2.0f;

    #pragma omp parallel for schedule(dynamic) default(none) \
    shared(pixels, offset_x, offset_y, max_iterations, params, center_y, center_x, scale_x)
    for (int y = 0; y < HEIGHT; ++y) {
        const float im_c = ((float)y - center_y) * scale_x + offset_y;

        for (int x = 0; x < WIDTH; ++x) {
            const float re_c = ((float)x - center_x) * scale_x + offset_x;

            const int iterations = mandelbrot_polynomial_iterations(re_c, im_c, max_iterations, params->alpha);
            pixels[y * WIDTH + x] = get_color_polynom_mandelbrot(iterations, max_iterations, params);
        }
    }

    UpdateTexture(params->texture, pixels);
}