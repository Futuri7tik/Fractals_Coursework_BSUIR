#include <math.h>

#include "functions.h"
#include "raylib.h"
#include "complex.h"

Color get_color_newton(int iter, int root, const NewtonParameters *params) {
    if (root == -1) return BLACK;

    float t = iter / 20.0f;
    if (t > 1.0f) t = 1.0f;

    unsigned char r_base = (unsigned char)(params->red * (root + 1) / 3.0f);
    unsigned char g_base = (unsigned char)(params->green * (root + 1) / 3.0f);
    unsigned char b_base = (unsigned char)params->blue;

    // Интерполяция к белому (255, 255, 255) в зависимости от t
    unsigned char r = (unsigned char)(r_base + (255 - r_base) * t);
    unsigned char g = (unsigned char)(g_base + (255 - g_base) * t);
    unsigned char b = (unsigned char)(b_base + (255 - b_base) * t);

    return (Color){ r, g, b, 255 };
}

int newton_iterations(float re_z, float im_z, const int max_iterations, int* root_num) {
    float complex z = re_z + im_z * I;
    int iterations;
    float complex roots[3] = {1.f + 0 * I, -0.5f + (SQRT3 / 2) * I, -0.5f - (SQRT3 / 2) * I};
    float eps = 1e-6f;

    *root_num = -1;
    for (iterations = 0; iterations < max_iterations; ++iterations) {
        if (cabsf(z) < eps / 100)
            break;

        float complex z3 =  z * z * z;
        float complex z2 = z * z;
        z = z - (z3 - 1) / (3 * z2);

        for (int r = 0; r < 3; ++r) {
            if (cabsf(z - roots[r]) < eps) {
                *root_num = r;
                return iterations;
            }
        }
    }

    return iterations;
}

Texture2D render_newton(float zoom, float offset_x, float offset_y, const int max_iterations,
                        NewtonParameters *params) {
    Image img = GenImageColor(WIDTH, HEIGHT, BLACK);
    Color* pixels = img.data;
    int root_num;

    #pragma omp parallel for schedule(dynamic) default(none) \
    shared(pixels, max_iterations, root_num, params, zoom, offset_x, offset_y)
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            const float re_z = ((float)x - (float)WIDTH / 2.0f) * (4.0f / ((float) WIDTH * zoom)) + offset_x;
            const float im_z = ((float)y - (float)HEIGHT / 2.0f) * (4.0f / ((float) WIDTH * zoom)) + offset_y;

            const int iterations = newton_iterations(re_z, im_z, max_iterations, &root_num);
            pixels[y * WIDTH + x] = get_color_newton(iterations, root_num, params);
        }
    }

    const Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);

    return texture;
}

