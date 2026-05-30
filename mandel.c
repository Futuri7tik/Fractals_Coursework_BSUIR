#include <bemapiset.h>
#include <tgmath.h>
#include "raylib.h"
#include "functions.h"

// получение количества итерационного процесс множества мандельброта для точки
static int mandelbrot_iterations(const float re_c, const float im_c, const int max_iterations) {
    const float q = (re_c - 0.25f) * (re_c - 0.25f) + im_c * im_c;
    // проверка на кардиоиду
    if (q * (q + (re_c - 0.25f)) < 0.25f * im_c * im_c) {
        return max_iterations;
    }
    if ((re_c + 1.0f) * (re_c + 1.0f) + im_c * im_c < 0.0625f) {
        return max_iterations;
    }

    // начальные значения мнимых и действительных частей
    int iterations = 0;
    float a = 0.0f, b = 0.0f, a2 = 0.0f, b2 = 0.0f, ab = a * b;

    // запуск итераций пока точка находится в круге радиуса 2
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

// получение количества итерационного процесса полиномиального множества мандельброта для точки
static int mandelbrot_polynomial_iterations(const float re_c, const float im_c, const int max_iterations,
    const float alph) {

    // начальные значения мнимых и действительных частей
    int iterations = 0;
    float a = 0.0f, b = 0.0f, a2 = 0.0f, b2 = 0.0f, ab;

    // нахождения радиуса видимости
    const float r = (2.0f > floorf(alph) + 1) ? 2.0f : floorf(a) + 1, r2 = r * r;

    // запуск итераций пока точка находится в круге радиуса 2
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

// получение цвета точки множества мандельброта
static Color get_color_mandelbrot(const int iteration, const int max_iterations, const MandelbrotParameters* params) {
    if (iteration == max_iterations) {
        return BLACK;
    }

    // получение множителя градиента
    const float t = (float)iteration / (float)max_iterations;

    // рассчет компонент
    const unsigned char r = (unsigned char)(params->red * (1-t) * t * t * t * 255),
    g = (unsigned char)(params->green * (1-t) * (1-t) * t * t * 255),
    b = (unsigned char)(params->blue * (1-t) * (1-t) * (1-t) * t * 255);

    return (Color) {r, g, b, 255};
}

// получение цвета точки полиномиального множества мандельброта
static Color get_color_polynom_mandelbrot(const int iteration, const int max_iterations, const PolynomMandelParameters* params) {
    if (iteration == max_iterations) {
        return BLACK;
    }

    // получение множителя градиента
    const float t = (float)iteration / (float)max_iterations;

    // рассчет компонент
    const unsigned char r = (unsigned char)(params->red * (1-t) * t * t * t * 255),
    g = (unsigned char)(params->green * (1-t) * (1-t) * t * t * 255),
    b = (unsigned char)(params->blue * (1-t) * (1-t) * (1-t) * t * 255);

    return (Color) {r, g, b, 255};
}

// рендер фрактала мандельброта в текстуру
void render_mandelbrot(float zoom, float offset_x,
                            float offset_y, const int max_iterations, const MandelbrotParameters *params) {
    // переменные для отрисовки фрактала. преобразования координат в комплексные
    static Color pixels[WIDTH * HEIGHT];
    const float scale_x = 4.0f / (WIDTH * zoom), center_x = WIDTH / 2.0f, center_y = HEIGHT / 2.0f;

    // распараллеливание вычислений через OpenMP
    #pragma omp parallel for schedule(dynamic) default(none) \
    shared(pixels, offset_x, offset_y, max_iterations, params, center_y, center_x, scale_x)

    // запуск вычислений для каждого пикселя
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

// рендер фрактала полиномиального мандельброта в текстуру
void render_polynom_mandel(float zoom, float offset_x, float offset_y,
    const int max_iterations, const PolynomMandelParameters *params) {
    // переменные для отрисовки фрактала. преобразования координат в комплексные
    static Color pixels[WIDTH * HEIGHT];
    const float scale_x = 4.0f / (WIDTH * zoom), center_x = WIDTH / 2.0f, center_y = HEIGHT / 2.0f;

    // распараллеливание вычислений через OpenMP
    #pragma omp parallel for schedule(dynamic) default(none) \
    shared(pixels, offset_x, offset_y, max_iterations, params, center_y, center_x, scale_x)

    // запуск вычислений для каждого пикселя
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