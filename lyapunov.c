#include "raylib.h"
#include <math.h>
#include <string.h>

#include "functions.h"

// === Вычисление показателя Ляпунова ===
float lyapunov_exponent(float rx, float ry, const char* sequence,
                        int iterations, float x0) {
    float x = x0;
    float sum = 0.0f;
    int len = strlen(sequence);

    // Пропускаем первые итерации (транзиент)
    for (int i = 0; i < 50; i++) {
        float r = (sequence[i % len] == 'A') ? rx : ry;
        x = r * x * (1.0f - x);
        if (x < 0.0f)
            x = 0.0f;

        if (x > 1.0f)
                x = 1.0f;
    }

    // Считаем показатель Ляпунова
    for (int i = 0; i < iterations; i++) {
        float r = (sequence[i % len] == 'A') ? rx : ry;
        x = r * x * (1.0f - x);

        // Защита от ln(0)
        float derivative = r * (1.0f - 2.0f * x);
        if (derivative < 0.0f)
            derivative = -derivative;

        if (derivative > 1e-10f) {
            sum += logf(derivative);
        }
    }

    return sum / iterations;
}

Texture2D render_lyapunov(const char* sequence, int iterations) {
    Image img = GenImageColor(WIDTH, HEIGHT, BLACK);
    Color* pixels = img.data;
    float scale_x = WIDTH / 2.0f, scale_y = HEIGHT / 2.0f;

    #pragma omp parallel for schedule(dynamic) default(none) \
    shared(sequence, iterations, pixels, scale_x, scale_y)
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            // Отображение координат в диапазон [2.0, 4.0]
            float rx = 2.0f + (float)x / scale_x;
            float ry = 2.0f + (float)y / scale_y;

            float lambda = lyapunov_exponent(rx, ry, sequence, iterations, 0.5f);

            // Раскраска
            if (lambda < 0) {
                // Зелёный (стабильно)
                float intensity = 1.0f - fabsf(lambda) * 2.0f;
                pixels[y * WIDTH + x] = (Color){
                    (unsigned char)(255 * intensity),(unsigned char)(255 * intensity),0,255
                };
            } else {
                // Синий (хаос)
                float intensity = lambda * 2.0f;
                pixels[y * WIDTH + x] = (Color){
                    0,0,(unsigned char)(255 * fminf(1.0f, intensity)),255
                };
            }
        }
    }

    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);
    return texture;
}