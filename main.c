#include "raylib.h"
#include "raymath.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#ifndef TEXT_TO_FLOAT_DEFINED
#define TEXT_TO_FLOAT_DEFINED
static float TextToFloat(const char *text) {
    return (float)atof(text);
}
#endif

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "functions.h"

#define WIDTH 1920
#define HEIGHT 1080

int main(void) {
    InitWindow(WIDTH, HEIGHT, "Fractal Gallery");
    bool needs_update = true;

    SetTargetFPS(144);

    AppState state = STATE_GALLERY;

    TreeParameters tree_params = {
        .depth = 14,
        .start_depth = 14,
        .angle = 25.0f * DEG2RAD,
        .length_factor = 0.75f
    };

    CarpetParameters carpet_params = {
        .depth = 7,
        .max_depth = 8,
        .start_length = HEIGHT - 200
    };

    TriangleParameters triangle_params = {
        .depth = 9,
        .max_depth = 13,
        .start_length = HEIGHT - 50
    };

    MandelbrotParameters mandel_params = {
        .iterations = 200,
        .max_iterations = 800,
        .zoom = 1.0f,
        .offset_x = 0.0f,
        .offset_y = 0.0f,
    };
    Texture2D mandelbrot_texture = {0};

    Camera2D camera = {0};
    camera.target = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
    camera.offset = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    float camera_offset = 20.0f;
    float tree_depth_float = 14.0f;
    float tree_angle_float = 25.0f;
    float tree_factor_float = 0.75f;
    float carpet_depth_float = 7.0f;
    float triangle_depth_float = 9.0f;
    float mandelbrot_iterations_float = (float)mandel_params.iterations;

    // === Главный цикл ===
    while (!WindowShouldClose()) {
        // === Обработка клавиш ===
        if (IsKeyPressed(KEY_ESCAPE)) {
            state = STATE_GALLERY;
        }

        // Навигация камеры (только в режиме рендера)
        if (state != STATE_GALLERY) {
            float current_speed = 20.0f / camera.zoom;
            if (IsKeyPressedRepeat(KEY_UP)) {
                camera.target.y -= current_speed;
                needs_update = true;
            }
            if (IsKeyPressedRepeat(KEY_DOWN)) {
                camera.target.y += current_speed;
                needs_update = true;
            }
            if (IsKeyPressedRepeat(KEY_LEFT)) {
                camera.target.x -= current_speed;
                needs_update = true;
            }
            if (IsKeyPressedRepeat(KEY_RIGHT)) {
                camera.target.x += current_speed;
                needs_update = true;
            }

            // Зум колесом мыши
            float wheel = GetMouseWheelMove();
            if (wheel > 0) {
                camera.zoom *= 1.05f;
                camera_offset *= 0.95f;
                needs_update = true;
            }
            else
                if (wheel < 0) {
                    camera.zoom *= 0.95f;
                    camera_offset *= 1.05f;
                    needs_update = true;
                }

            // Ограничения камеры
            // if (camera.zoom < 0.1f)
            //     camera.zoom = 0.1f;
            // if (camera.zoom > 100.0f)
            //     camera.zoom = 100.0f;
            // if (camera_offset < 1.0f)
            //     camera_offset = 1.0f;
            // if (camera_offset > 1000.0f)
            //     camera_offset = 1000.0f;
        }

        // === Отрисовка ===
        BeginDrawing();
        ClearBackground(BLACK);

        if (state == STATE_GALLERY) {
            GuiPanel((Rectangle){WIDTH/2.0f - 600, HEIGHT/2.0f - 400, 1200, 800}, "Fractal Gallery");
            camera.zoom = 1.0f;
            camera.target = (Vector2){WIDTH/2.0f, HEIGHT/2.0f};
            camera_offset = 20.0f;

            if (GuiButton((Rectangle){WIDTH/2.0f - 250, HEIGHT/2.0f - 100, 500, 50}, "Pythagoras Tree")) {
                state = STATE_TREE;
                // Синхронизируем временные переменные с параметрами
                tree_depth_float = (float)tree_params.depth;
                tree_angle_float = tree_params.angle * RAD2DEG;
                tree_factor_float = tree_params.length_factor;
            }
            if (GuiButton((Rectangle){WIDTH/2.0f - 250, HEIGHT/2.0f - 30, 500, 50}, "Sierpinski Carpet")) {
                state = STATE_CARPET;
                carpet_depth_float = (float)carpet_params.depth;
            }
            if (GuiButton((Rectangle){WIDTH/2.0f - 250, HEIGHT/2.0f + 40, 500, 50}, "Sierpinski Triangle")) {
                state = STATE_TRIANGLE;
                triangle_depth_float = (float)triangle_params.depth;
            }
            if (GuiButton((Rectangle){WIDTH/2.0f - 250, HEIGHT/2.0f + 110, 500, 50}, "Mandelbrot Set")) {
                state = STATE_MANDELBROT;
                needs_update = true;
                mandel_params.zoom = camera.zoom;
                camera.target = (Vector2){WIDTH / 2.0f + 300.0f, HEIGHT / 2.0f};
                mandel_params.offset_x = (camera.target.x - WIDTH/2.0f) / WIDTH;
                mandel_params.offset_y = (camera.target.y - HEIGHT/2.0f) / HEIGHT;
            }
            DrawText("Press ESC to return to gallery", WIDTH/2 - 150, HEIGHT - 50, 16, GRAY);

        }
        else {
            BeginMode2D(camera);

            switch (state) {
                case STATE_TREE:
                    draw_tree(WIDTH/2.0f, HEIGHT - 220, 200, -PI/2.0f,
                         tree_params.depth, &tree_params);
                    break;
               case STATE_CARPET: {
                   float x_start = (WIDTH - carpet_params.start_length) / 2.0f;
                   float y_start = (HEIGHT - carpet_params.start_length) / 2.0f;
                   draw_square(x_start, y_start, carpet_params.start_length, VIOLET);
                   draw_carpet(x_start, y_start, carpet_params.start_length,
                              carpet_params.depth, &carpet_params);
                   break;
               }
               case STATE_TRIANGLE: {
                   float start_height = triangle_params.start_length * sqrtf(3.0f) / 2.0f;
                   float x_start = (WIDTH - triangle_params.start_length) / 2.0f;
                   float y_start = HEIGHT - (HEIGHT - start_height) / 2.0f;
                   draw_triangle_base(x_start, y_start, triangle_params.start_length, YELLOW);
                   draw_sierpinski_triangle(x_start, y_start, triangle_params.start_length,
                                           triangle_params.depth, &triangle_params);
                   break;
               }
                case STATE_MANDELBROT: {
                   EndMode2D();

                   if (needs_update == true) {
                       if (mandelbrot_texture.id > 0)
                           UnloadTexture(mandelbrot_texture);
                       mandel_params.zoom = camera.zoom;
                       mandel_params.offset_x = (camera.target.x - WIDTH / 1.5f) * (4.0f / WIDTH);
                       mandel_params.offset_y = (camera.target.y - HEIGHT / 2.0f) * (4.0f / WIDTH);

                       mandelbrot_texture = render_mandelbrot(WIDTH, HEIGHT,
                                              mandel_params.zoom,
                                              mandel_params.offset_x,
                                              mandel_params.offset_y,
                                              mandel_params.iterations);
                       needs_update = false;
                   }

                   if (mandelbrot_texture.id > 0)
                       DrawTexture(mandelbrot_texture, 0, 0, WHITE);

                   BeginMode2D(camera);
                   break;
               }
            }


            EndMode2D();

            // === UI ПАНЕЛЬ ===
            GuiPanel((Rectangle){10, 10, 280, 380}, "Controls");

            switch (state) {
                case STATE_TREE:
                    GuiLabel((Rectangle){20, 50, 200, 20}, TextFormat("Depth: %d", tree_params.depth));

                    GuiSlider((Rectangle){20, 80, 200, 20}, NULL, NULL,
                        &tree_depth_float, 1,18);
                    tree_params.depth = (int)tree_depth_float;

                    float angle_deg = tree_params.angle * RAD2DEG;
                    GuiLabel((Rectangle){20, 120, 200, 20}, TextFormat("Angle: %.1f°", angle_deg));
                    GuiSlider((Rectangle){20, 150, 200, 20}, NULL, NULL, &angle_deg, 0, 90);
                    tree_params.angle = angle_deg * DEG2RAD;

                    GuiLabel((Rectangle){20, 190, 200, 20}, TextFormat("Length Factor: %.2f", tree_params.length_factor));
                    GuiSlider((Rectangle){20, 220, 200, 20}, NULL, NULL, &tree_factor_float, 0, 0.9);
                    tree_params.length_factor = tree_factor_float;

                    if (GuiButton((Rectangle){20, 260, 110, 30}, "Reset")) {
                        tree_params.depth = 14;
                        tree_params.angle = 25.0f * DEG2RAD;
                        tree_params.length_factor = 0.75f;
                        tree_depth_float = (float) tree_params.depth;
                        tree_angle_float = 25.0f;
                        tree_factor_float = 0.75f;
                        camera.zoom = 1.0f;
                        camera.target = (Vector2){WIDTH/2.0f, HEIGHT/2.0f};
                        camera_offset = 20.0f;
                    }
                    break;
                case STATE_CARPET:
                    GuiLabel((Rectangle){20, 50, 200, 20}, TextFormat("Depth: %d", carpet_params.depth));
                    GuiSlider((Rectangle){20, 80, 200, 20}, NULL, NULL, &carpet_depth_float, 0, 8);
                    carpet_params.depth = (int)carpet_depth_float;

                    if (GuiButton((Rectangle){20, 120, 110, 30}, "Reset")) {
                        carpet_params.depth = 7;
                        carpet_depth_float = 7.0f;
                        camera.zoom = 1.0f;
                        camera.target = (Vector2){WIDTH/2.0f, HEIGHT/2.0f};
                        camera_offset = 20.0f;
                    }
                    break;
                case STATE_TRIANGLE:
                    GuiLabel((Rectangle){20, 50, 200, 20}, TextFormat("Depth: %d", triangle_params.depth));
                    GuiSlider((Rectangle){20, 80, 200, 20}, NULL, NULL, &triangle_depth_float, 0, 13);
                    triangle_params.depth = (int)triangle_depth_float;

                    if (GuiButton((Rectangle){20, 120, 110, 30}, "Reset")) {
                        triangle_params.depth = 9;
                        triangle_depth_float = 9.0f;
                        camera.zoom = 1.0f;
                        camera.target = (Vector2){WIDTH/2.0f, HEIGHT/2.0f};
                        camera_offset = 20.0f;
                    }
                    break;
                case STATE_MANDELBROT:
                    GuiLabel((Rectangle){20, 50, 200, 20}, TextFormat("Iterations: %d", mandel_params.iterations));
                    if (GuiSlider((Rectangle){20, 80, 200, 20}, NULL, NULL,
                        &mandelbrot_iterations_float, 0, 300)) {

                        mandel_params.iterations = (int)mandelbrot_iterations_float;
                        needs_update = true;
                    }

                    if (GuiButton((Rectangle){20, 160, 110, 30}, "Reset")) {
                        mandel_params.iterations = 200;
                        mandelbrot_iterations_float = (float)mandel_params.iterations;
                        mandel_params.max_iterations = 800;
                        mandel_params.zoom = 1.0f;
                        mandel_params.offset_x = 0.0f;
                        mandel_params.offset_y = 0.0f;
                        camera.target = (Vector2){WIDTH / 2.0f + 300.0f, HEIGHT / 2.0f};
                        camera.offset = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
                        camera.rotation = 0.0f;
                        camera.zoom = 1.0f;
                        needs_update = true;
                    }
                    break;
            }

            if (GuiButton((Rectangle){20, 330, 110, 30}, "<- Gallery")) {
                state = STATE_GALLERY;
            }
        }

        // DrawFPS(WIDTH - 100, HEIGHT - 30);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}