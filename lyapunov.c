#include "raylib.h"
#include <math.h>
#include <string.h>

#include "functions.h"

// вычисление экспоненты ляпунова
static float lyapunov_exponent(float rx, float ry, const char* sequence, int seq_len,
                        int iterations, float x0) {
    float x = x0;
    float sum = 0.0f;

    // запись последовательности
    float r_seq[64];
    for (int i = 0; i < seq_len; i++) {
        r_seq[i] = (sequence[i] == 'A') ? rx : ry;
    }

    int seq_idx = 0;

    // Пропускаем первые итерации (транзиент)
    for (int i = 0; i < 50; i++) {
        float r = r_seq[seq_idx++];
        if (seq_idx == seq_len)
            seq_idx = 0;

        x = r * x * (1.0f - x);

        if (x < 0.0f)
            x = 0.0f;
        else
            if (x > 1.0f)
                x = 1.0f;
    }

    // считаем показатель Ляпунова
    for (int i = 0; i < iterations; i++) {
        float r = r_seq[seq_idx++];
        if (seq_idx == seq_len) seq_idx = 0;

        x = r * x * (1.0f - x);

        float derivative = r * (1.0f - 2.0f * x);
        derivative = fabsf(derivative);

        if (derivative > 1e-10f) {
            sum += logf(derivative);
        }
    }

    return sum / iterations;
}

// рендер фрактала Ляпунова в текстуру
Texture2D render_lyapunov(LyapunovParameters* params) {
    // создание изображения для текстуры
    Image img = GenImageColor(WIDTH, HEIGHT, BLACK);
    Color* pixels = img.data;

    // параметры преобразования координат
    float scale_x = WIDTH / 2.0f, scale_y = HEIGHT / 2.0f;
    int seq_len = strlen(params->sequence);

    // параметры рендера
    char* sequence = params->sequence;
    float iterations = params->iterations, red = params->red, green = params->green, blue = params->blue;

    // распараллеливание через OpenMP
    #pragma omp parallel for schedule(dynamic) default(none) \
    shared(sequence, iterations, pixels, scale_x, scale_y, seq_len, red, green, blue)

    // запуск итераций для каждой точки экрана
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            // отображение координат в диапазон [2.0, 4.0]
            float rx = 2.0f + (float)x / scale_x, ry = 2.0f + (float)y / scale_y;

            float lambda = lyapunov_exponent(rx, ry, sequence, seq_len,(int) iterations, 0.5f);

            // раскраска
            if (lambda < 0) {
                float intensity = 1.0f - fabsf(lambda) * 2.0f;
                pixels[y * WIDTH + x] = (Color){
                    (unsigned char)(255 * intensity),(unsigned char)(255 * intensity),(unsigned char)blue,255
                };
            }
            else {
                float intensity = lambda * 2.0f;
                pixels[y * WIDTH + x] = (Color){
                    (unsigned char)red,(unsigned char)blue,(unsigned char)(255 * fminf(1.0f, intensity)),255
                };
            }
        }
    }

    // получение финальной текстуры
    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);

    return texture;
}