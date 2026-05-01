#include <math.h>
#include "raylib.h"
#include "functions.h"

static Vector2 draw_f(float x, float y, float angle, float length, Color color);
static Vector2 draw_x(int depth, float x, float y, float* angle, float tilt_angle, float length, Color color);
static Vector2 draw_y(int depth, float x, float y, float* angle, float tilt_angle, float length, Color color);

// drawing line
static Vector2 draw_f(float x, float y, float angle, float length, Color color) {
    float next_x = x + cosf(angle * DEG2RAD) * length;
    float next_y = y + sinf(angle * DEG2RAD) * length;
    DrawLineEx((Vector2){x, y}, (Vector2){next_x, next_y}, 1.0f, color);

    return (Vector2){next_x, next_y};
}

// y -> - fx - y
Vector2 draw_y(int depth, float x, float y, float* angle, float tilt_angle, float length, Color color) {
    if (depth <= 0)
        return (Vector2){x, y};

    *angle -= tilt_angle;

    Vector2 new_pos = draw_f(x, y, *angle, length, color);
    new_pos = draw_x(depth - 1, new_pos.x, new_pos.y, angle, tilt_angle, length, color);

    *angle -= 90;

    new_pos = draw_y(depth - 1, new_pos.x, new_pos.y, angle, tilt_angle, length, color);

    return new_pos;
}

// x -> x + yf +
Vector2 draw_x(int depth, float x, float y, float* angle, float tilt_angle,float length, Color color) {
    if (depth <= 0)
        return (Vector2) {x, y};

    Vector2 new_pos = draw_x(depth - 1, x, y, angle, tilt_angle, length, color);
    *angle += 90;

    new_pos = draw_y(depth - 1, new_pos.x, new_pos.y, angle, tilt_angle, length, color);
    new_pos = draw_f(new_pos.x, new_pos.y, *angle, length, color);

    *angle += 90;

    return new_pos;
}

void draw_dragon(float x_start, float y_start, float* start_angle, float tilt_angle,float length, int depth, Color color) {
    Vector2 pos = draw_f(x_start, y_start, *start_angle, length, color);
    draw_x(depth - 1, pos.x, pos.y, start_angle, tilt_angle, length, color);
}

// int m(void) {
//     InitWindow(WIDTH, HEIGHT, "Fractal Gallery");
//     SetTargetFPS(144);
//
//     Camera2D camera = {0};
//     camera.target = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
//     camera.offset = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
//     camera.rotation = 0.0f;
//     camera.zoom = 1.0f;
//     DragonParameters dragon = {
//         13,
//         14,
//         WIDTH / 2.0f,
//         HEIGHT / 2.0f,
//         0,
//         90,
//         10,
//         255,
//         255,
//         255
//     };
//
//     while (!WindowShouldClose()) {
//         BeginDrawing();
//         ClearBackground(BLACK);
//         BeginMode2D(camera);
//
//         dragon.angle = 0;
//         draw_dragon(dragon.x, dragon.y, &dragon.angle, dragon.length, (int) dragon.depth,
//             (Color){dragon.red, dragon.green, dragon.blue,255});
//
//         EndMode2D();
//         EndDrawing();
//     }
//
//     return 0;
// }

