#include "raylib.h"
#include "functions.h"

void draw_carpet(Image* img, float x, float y, float length, int depth) {
    if (depth < 0)
        return;

    float new_length = length / 3.0f;
    float x_draw = x + new_length;
    float y_draw = y + new_length;

    ImageDrawRectangleV(img, (Vector2) {x_draw, y_draw}, (Vector2) {new_length, new_length}, BLACK);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i != 1 || j != 1) {
                draw_carpet(img, x + i * new_length, y + j * new_length,
                                    new_length, depth - 1);
            }
        }
    }
}

void render_carpet(int depth, float start_length, Color color, const CarpetParameters* params) {

    Image img = GenImageColor(WIDTH, HEIGHT, BLACK);

    float x_start = ((float) WIDTH - start_length) / 2.0f;
    float y_start = ((float) HEIGHT - start_length) / 2.0f;

    ImageDrawRectangleV(&img, (Vector2) {x_start, y_start}, (Vector2) {start_length, start_length}, color);
    draw_carpet(&img, x_start, y_start, start_length, depth);

    UpdateTexture(params->texture, img.data);
}