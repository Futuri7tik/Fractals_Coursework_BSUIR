#include "raylib.h"
#include "stdio.h"
#include "functions.h"

void draw_square(float x, float y, float length, Color color) {
    DrawRectangle(x, y, length, length, color);
}

Color get_color_carpet(const int depth, CarpetParameters* params) {
    const int color_1[3] = {0, 0, 0};
    const int color_2[3] = {0, 0, 153};
    const float t = 1.0f - (float) depth / (float) params->max_depth;
    const unsigned char r = color_1[0] * (1 - t) + color_2[0] * t;
    const unsigned char g = color_1[1] * (1 - t) + color_2[1] * t;
    const unsigned char b = color_1[2] * (1 - t) + color_2[2] * t;
    return (Color){r, g, b, 255};
}

void draw_carpet(float x_left, float y_left, float length,
                 int depth, CarpetParameters* params) {
    depth -= 1;
    if (depth < 0) return;

    if (x_left + length < 0 || x_left > GetScreenWidth() ||
        y_left + length < 0 || y_left > GetScreenHeight()) {
        return;
    }

    float new_length = length / 3.0f;
    float x_draw = x_left + new_length;
    float y_draw = y_left + new_length;

    Color color = get_color_carpet(depth, params);
    draw_square(x_draw, y_draw, new_length, color);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i != 1 || j != 1) {
                draw_carpet(x_left + (float) i * new_length, y_left + (float) j * new_length,
                           new_length, depth, params);
            }
        }
    }
}


void draw_carpet_to_image(Image* img, float x, float y, float length, int depth) {
    if (depth < 0) return;

    float new_length = length / 3.0f;
    float x_draw = x + new_length;
    float y_draw = y + new_length;

    // Вырезаем центральный квадрат (чёрный)
    ImageDrawRectangle(img, x_draw, y_draw, new_length, new_length, BLACK);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i != 1 || j != 1) {
                draw_carpet_to_image(img, x + i * new_length, y + j * new_length,
                                    new_length, depth - 1);
            }
        }
    }
}

Texture2D render_carpet_to_texture(int width, int height, int depth, float start_length) {
    Image img = GenImageColor(width, height, BLACK);

    // Рисуем основание
    float x_start = ((float) width - start_length) / 2.0f;
    float y_start = ((float) height - start_length) / 2.0f;
    ImageDrawRectangle(&img, (int) x_start, (int) y_start, (int) start_length, (int) start_length, VIOLET);

    // Рекурсивно рисуем ковёр
    draw_carpet_to_image(&img, x_start, y_start, start_length, depth);

    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);
    return texture;
}