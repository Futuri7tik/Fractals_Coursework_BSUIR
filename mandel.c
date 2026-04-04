#include "raylib.h"
#include "functions.h"

int mandelbrot_iterations(const float re_c, const float im_c, const int max_iterations) {
    int iterations = 0;
    float a = 0.0f, b = 0.0f;

    while (a * a + b * b < 4.0f && iterations < max_iterations) {
        const float a1 = a * a - b * b + re_c;
        const float b1 = 2 * a * b + im_c;

        a = a1;
        b = b1;
        iterations++;
    }

    return iterations;
}

Color get_color_mandelbrot(const int iteration, const int max_iterations) {
    if (iteration == max_iterations) {
        return BLACK;
    }

    // Более яркие цвета
    unsigned char r = iteration * 4 % 255;
    unsigned char g = iteration * 8 % 255;
    unsigned char b = iteration * 12 % 255;

    return (Color) {r, g, b, 255};
}

Texture2D render_mandelbrot(const int width, const int height, float zoom, float offset_x,
    float offset_y, const int max_iterations) {
    Image img = GenImageColor(width, height, BLACK);
    Color* pixels = (Color*)img.data;

    #pragma omp parallel for schedule(dynamic)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const float re_c = ((float)x - (float)width / 1.5f) * (4.0f / ((float) width * zoom)) + offset_x;
            const float im_c = ((float)y - (float)height / 2.0f) * (4.0f / ((float) width * zoom)) + offset_y;

            const int iterations = mandelbrot_iterations(re_c, im_c, max_iterations);
            pixels[y * width + x] = get_color_mandelbrot(iterations, max_iterations);
        }
    }

    const Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);

    return texture;
}