#include <stdlib.h>
#include "functions.h"
#include "raylib.h"

Texture2D render_barnsley_fern(Vector2 pos, const int depth, const FernParameters params) {
    Image img = GenImageColor(WIDTH, HEIGHT, BLACK);
    float a, b, c, d, e, f;

    for (int i = 0; i < depth; ++i) {
        int possib = GetRandomValue(0, 99);
        const int scale = 100;
        const float offset_x = WIDTH / 2.0f;
        const float offset_y = HEIGHT - 50.0f;

        const float aff_trans[4][6] = {
            {0, 0, 0, 0.16f, 0, 0}, {0.85f, 0.04f, -0.04f, 0.85f, 0, 1.6f},
            {0.2f, -0.26f, 0.23f, 0.22f, 0.f, 1.6f}, {-0.15f, 0.28f, 0.26f, 0.24f, 0.f, 0.44f}
        };

        if (possib <= (int) params.prob1) {
            a = aff_trans[0][0], b = aff_trans[0][1], c = aff_trans[0][2], d = aff_trans[0][3],
            e = aff_trans[0][4], f = aff_trans[0][5];
        }
        else
            if (possib <= (int) params.prob1 + (int) params.prob2) {
                a = aff_trans[1][0], b = aff_trans[1][1], c = aff_trans[1][2], d = aff_trans[1][3],
                e = aff_trans[1][4], f = aff_trans[1][5];
            }
            else
                if (possib <= (int) params.prob1 + (int) params.prob2 + (int) params.prob3) {
                    a = aff_trans[2][0], b = aff_trans[2][1], c = aff_trans[2][2], d = aff_trans[2][3],
                    e = aff_trans[2][4], f = aff_trans[2][5];
                }
                else {
                    a = aff_trans[3][0], b = aff_trans[3][1], c = aff_trans[3][2], d = aff_trans[3][3],
                    e = aff_trans[3][4], f = aff_trans[3][5];
                }

        const float new_x = a * pos.x + b * pos.y + e;
        const float new_y = c * pos.x + d * pos.y + f;

        pos.x = new_x;
        pos.y = new_y;

        ImageDrawPixel(&img, pos.x * scale + offset_x, offset_y - pos.y * scale, GREEN);
    }

    const Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);

    return texture;
}