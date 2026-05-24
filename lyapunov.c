#include "raylib.h"
#include <math.h>
#include <string.h>

#include "functions.h"

// === Вычисление показателя Ляпунова ===
// === Оптимизированное вычисление показателя Ляпунова ===
float lyapunov_exponent(float rx, float ry, const char* sequence, int seq_len,
                        int iterations, float x0) {
    float x = x0;
    float sum = 0.0f;

    // Предвычисляем множители для текущего пикселя, чтобы не парсить строку в цикле
    // Массив на 64 элемента обычно покрывает любые разумные длины паттернов (AB, AABAB и т.д.)
    float r_seq[64];
    for (int i = 0; i < seq_len; i++) {
        r_seq[i] = (sequence[i] == 'A') ? rx : ry;
    }

    int seq_idx = 0;

    // Пропускаем первые итерации (транзиент)
    for (int i = 0; i < 50; i++) {
        float r = r_seq[seq_idx++];
        if (seq_idx == seq_len) seq_idx = 0; // Замена медленного %

        x = r * x * (1.0f - x);

        if (x < 0.0f) x = 0.0f;
        else if (x > 1.0f) x = 1.0f;
    }

    // Считаем показатель Ляпунова
    for (int i = 0; i < iterations; i++) {
        float r = r_seq[seq_idx++];
        if (seq_idx == seq_len) seq_idx = 0;

        x = r * x * (1.0f - x);

        float derivative = r * (1.0f - 2.0f * x);
        derivative = fabsf(derivative); // Быстрое снятие знака без ветвления

        if (derivative > 1e-10f) {
            sum += logf(derivative);
        }
    }

    return sum / iterations;
}

Texture2D render_lyapunov(LyapunovParameters* params) {
    Image img = GenImageColor(WIDTH, HEIGHT, BLACK);
    Color* pixels = img.data;
    float scale_x = WIDTH / 2.0f, scale_y = HEIGHT / 2.0f;
    int seq_len = strlen(params->sequence);
    char* sequence = params->sequence;
    float iterations = params->iterations, red = params->red, green = params->green, blue = params->blue;

    #pragma omp parallel for schedule(dynamic) default(none) \
    shared(sequence, iterations, pixels, scale_x, scale_y, seq_len, red, green, blue)
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            // Отображение координат в диапазон [2.0, 4.0]
            float rx = 2.0f + (float)x / scale_x;
            float ry = 2.0f + (float)y / scale_y;

            float lambda = lyapunov_exponent(rx, ry, sequence, seq_len,(int) iterations, 0.5f);

            // Раскраска
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

    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);
    return texture;
}