#include "raylib.h"
#include "functions.h"

void draw_carpet_to_image(Image* img, float x, float y, float length, int depth) {
    if (depth < 0) return;

    float new_length = length / 3.0f;
    float x_draw = x + new_length;
    float y_draw = y + new_length;

    ImageDrawRectangleV(img, (Vector2) {x_draw, y_draw}, (Vector2) {new_length, new_length}, BLACK);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i != 1 || j != 1) {
                draw_carpet_to_image(img, x + i * new_length, y + j * new_length,
                                    new_length, depth - 1);
            }
        }
    }
}

Texture2D render_carpet_to_texture(int width, int height, int depth, float start_length, Color color) {
    Image img = GenImageColor(width, height, BLACK);

    float x_start = ((float) width - start_length) / 2.0f;
    float y_start = ((float) height - start_length) / 2.0f;

    ImageDrawRectangleV(&img, (Vector2) {x_start, y_start}, (Vector2) {start_length, start_length}, color);
    // Рекурсивно рисуем ковёр
    draw_carpet_to_image(&img, x_start, y_start, start_length, depth);

    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);
    return texture;
}