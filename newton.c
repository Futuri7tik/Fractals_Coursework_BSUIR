#include <math.h>

#include "functions.h"
#include "raylib.h"


Color get_color_newton(int iter, int root, const NewtonParameters *params) {
    if (root == -1) return BLACK;

    float t = iter / 30.0f;
    if (t > 1.0f) t = 1.0f;

    unsigned char r_base = (unsigned char)(params->red * (root + 1) / 3.0f);
    unsigned char g_base = (unsigned char)(params->green * (root + 1) / 3.0f);
    unsigned char b_base = (unsigned char)params->blue;

    unsigned char r = (unsigned char)(r_base + (params->gradient_r - r_base) * t);
    unsigned char g = (unsigned char)(g_base + (params->gradient_g - g_base) * t);
    unsigned char b = (unsigned char)(b_base + (params->gradient_b - b_base) * t);

    return (Color){ r, g, b, 255 };
}

int newton_iterations(float re_z, float im_z, const int max_iterations, int* root_num) {
    int iterations;
    float eps = 1e-4f;
    static float roots[3][2] = {{1, 0}, {-0.5f, -SQRT3 / 2}, {-0.5f, SQRT3 / 2}};
    *root_num = -1;

    for (iterations = 0; iterations < max_iterations; ++iterations) {
        float re2 = re_z * re_z;
        float im2 = im_z * im_z;
        float r2 = re2 + im2; // abs(z)^2

        if (r2 == 0)
            break;

        // 3 * (a + bi)^2 = 3 * (a2 - b2 + 2ab)
        // 1 / (a + bi) = (a - bi) / (a2 + b2)
        float denom_re = 3.0f * (re2 - im2);
        float denom_im = 6.0f * re_z * im_z;
        float denom = denom_re * denom_re + denom_im * denom_im;

        // z - (z3 - 1) / (3 * z2) = 2z/3 + 1/(3 * z2)
        float next_re_z = (2.0f / 3.0f) * re_z + denom_re / denom;
        float next_im_z = (2.0f / 3.0f) * im_z - denom_im / denom;

        for (int r = 0; r < 3; ++r) {
            float re_check = next_re_z - roots[r][0];
            float im_check = next_im_z - roots[r][1];
            if (re_check * re_check + im_check * im_check < eps * eps) {
                *root_num = r;
                return iterations;
            }
        }
        re_z = next_re_z;
        im_z = next_im_z;
    }

    return iterations;
}

Texture2D render_newton(float zoom, float offset_x, float offset_y, const int max_iterations, NewtonParameters *params) {
    Image img = GenImageColor(WIDTH, HEIGHT, BLACK);
    Color* pixels = img.data;

    #pragma omp parallel for schedule(dynamic) default(none) \
    shared(pixels, max_iterations, params, zoom, offset_x, offset_y)
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int root_num;
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

