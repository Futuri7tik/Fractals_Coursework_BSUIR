#include "raylib.h"
#include "functions.h"

// отрисовка ковра в текстуру
static void draw_carpet(Image* img, float x, float y, float length, int depth) {
    // базовый случай рекурсии
    if (depth < 0)
        return;

    // параметры квадраты
    float new_length = length / 3.0f, x_draw = x + new_length, y_draw = y + new_length;

    // отрисовка центрального квадрата
    ImageDrawRectangleV(img, (Vector2) {x_draw, y_draw}, (Vector2) {new_length, new_length}, BLACK);

    // рекурсивный вызов для всех 8 квадратов после центрального
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i != 1 || j != 1) {
                draw_carpet(img, x + i * new_length, y + j * new_length,
                                    new_length, depth - 1);
            }
        }
    }
}

// рендер ковра
void render_carpet(int depth, float start_length, Color color, const CarpetParameters* params) {
    // создание изображения для текстуры
    Image img = GenImageColor(WIDTH, HEIGHT, BLACK);

    // начальная координата квадрата
    float x_start = ((float) WIDTH - start_length) / 2.0f, y_start = ((float) HEIGHT - start_length) / 2.0f;

    // базовый квадрат
    ImageDrawRectangleV(&img, (Vector2) {x_start, y_start}, (Vector2) {start_length, start_length}, color);

    // вызов отрисовки
    draw_carpet(&img, x_start, y_start, start_length, depth);

    UpdateTexture(params->texture, img.data);
}