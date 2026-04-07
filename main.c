#include "raylib.h"
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


int main(void) {
    InitWindow(WIDTH, HEIGHT, "Fractal Gallery");
    bool needs_update = true;

    SetTargetFPS(144);

    AppState state = STATE_GALLERY;

    TreeParameters tree_params;
    CarpetParameters carpet_params;
    TriangleParameters triangle_params;
    MandelbrotParameters mandel_params;

    init_tree_parameters(&tree_params);
    init_carpet_parameters(&carpet_params);
    init_triangle_parameters(&triangle_params);
    init_mandelbrot_parameters(&mandel_params);

    Camera2D camera = {0};
    camera.target = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
    camera.offset = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    float camera_offset = 20.0f;

    // float mandelbrot_iterations_float = (float)mandel_params.iterations;

    // === Главный цикл ===
    while (!WindowShouldClose()) {
        // === Обработка клавиш ===
        if (IsKeyPressed(KEY_TAB)) {
            state = STATE_GALLERY;
        }

        // Навигация камеры
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
            }
            if (GuiButton((Rectangle){WIDTH/2.0f - 250, HEIGHT/2.0f - 30, 500, 50}, "Sierpinski Carpet")) {
                state = STATE_CARPET;
                needs_update = true;
            }
            if (GuiButton((Rectangle){WIDTH/2.0f - 250, HEIGHT/2.0f + 40, 500, 50}, "Sierpinski Triangle")) {
                state = STATE_TRIANGLE;
            }
            if (GuiButton((Rectangle){WIDTH/2.0f - 250, HEIGHT/2.0f + 110, 500, 50}, "Mandelbrot Set")) {
                state = STATE_MANDELBROT;
                needs_update = true;
                mandel_params.zoom = camera.zoom;
                camera.target = (Vector2){WIDTH / 2.0f - 300.0f, HEIGHT / 2.0f};
                mandel_params.offset_x = (camera.target.x - WIDTH/2.0f) / WIDTH;
                mandel_params.offset_y = (camera.target.y - HEIGHT/2.0f) / HEIGHT;
            }
            DrawText("Press ESC to exit", WIDTH/2 - 150, HEIGHT - 50, 16, GRAY);

        }
        else {
            BeginMode2D(camera);

            switch (state) {
                case STATE_TREE:
                    draw_tree(WIDTH/2.0f, HEIGHT - 220, 200, -PI/2.0f,
                         (int) tree_params.depth, &tree_params);
                    break;
               case STATE_CARPET: {
                   if (needs_update) {
                       if (carpet_params.texture.id > 0)
                           UnloadTexture(carpet_params.texture);
                       carpet_params.texture = render_carpet_to_texture(WIDTH, HEIGHT,
                                                  (int) carpet_params.depth,
                                                  carpet_params.start_length);
                       needs_update = false;
                   }

                   if (carpet_params.texture.id > 0)
                       DrawTexture(carpet_params.texture, 0, 0, WHITE);

                   break;
               }
               case STATE_TRIANGLE: {
                   float start_height = triangle_params.start_length * sqrtf(3.0f) / 2.0f;
                   float x_start = (WIDTH - triangle_params.start_length) / 2.0f;
                   float y_start = HEIGHT - (HEIGHT - start_height) / 2.0f;
                   draw_triangle_base(x_start, y_start, triangle_params.start_length, YELLOW);
                   draw_sierpinski_triangle(x_start, y_start, triangle_params.start_length,
                                           (int) triangle_params.depth, &triangle_params);
                   break;
               }
                case STATE_MANDELBROT: {
                   EndMode2D();

                   if (needs_update == true) {
                       if (mandel_params.texture.id > 0)
                           UnloadTexture(mandel_params.texture);
                       mandel_params.zoom = camera.zoom;
                       mandel_params.offset_x = (camera.target.x - WIDTH / 2.0f) * (4.0f / WIDTH);
                       mandel_params.offset_y = (camera.target.y - HEIGHT / 2.0f) * (4.0f / WIDTH);

                       mandel_params.texture = render_mandelbrot(WIDTH, HEIGHT,
                                              mandel_params.zoom,
                                              mandel_params.offset_x,
                                              mandel_params.offset_y,
                                              (int) mandel_params.iterations,
                                              &mandel_params);
                       needs_update = false;
                   }

                   if (mandel_params.texture.id > 0)
                       DrawTexture(mandel_params.texture, 0, 0, WHITE);

                   BeginMode2D(camera);
                   break;
               }
            }


            EndMode2D();

            // === UI ПАНЕЛЬ ===
            GuiPanel((Rectangle){10, 10, 280, 380}, "Controls");

            switch (state) {
                case STATE_TREE:
                    GuiLabel((Rectangle){20, 50, 200, 20}, TextFormat("Depth: %d", (int) tree_params.depth));

                    GuiSlider((Rectangle){20, 80, 200, 20}, NULL, NULL,
                        &tree_params.depth, 1,(float)tree_params.max_depth);
                    tree_params.depth = (float) (int)tree_params.depth;

                    GuiLabel((Rectangle){20, 120, 200, 20}, TextFormat("Angle: %.1f°", tree_params.angle_degrees));
                    GuiSlider((Rectangle){20, 150, 200, 20}, NULL, NULL,
                        &tree_params.angle_degrees, 0, 90);
                    tree_params.angle = tree_params.angle_degrees * DEG2RAD;

                    GuiLabel((Rectangle){20, 190, 200, 20}, TextFormat("Length Factor: %.2f", tree_params.length_factor));
                    GuiSlider((Rectangle){20, 220, 200, 20}, NULL, NULL,
                        &tree_params.length_factor, 0, 0.9f);

                    if (GuiButton((Rectangle){20, 260, 110, 30}, "Reset")) {
                        tree_params.depth = 14;
                        tree_params.angle = 25.0f * DEG2RAD;
                        tree_params.angle_degrees = 25.0f;
                        tree_params.length_factor = 0.75f;
                        camera.zoom = 1.0f;
                        camera.target = (Vector2){WIDTH/2.0f, HEIGHT/2.0f};
                        camera_offset = 20.0f;
                    }
                    break;
                case STATE_CARPET:
                    GuiLabel((Rectangle){20, 50, 200, 20}, TextFormat("Depth: %d", (int) carpet_params.depth));
                    if (GuiSlider((Rectangle){20, 80, 200, 20}, NULL, NULL,
                        &carpet_params.depth, 0, (float) carpet_params.max_depth))
                        needs_update = true;

                    carpet_params.depth = (float) (int) carpet_params.depth;

                    if (GuiButton((Rectangle){20, 120, 110, 30}, "Reset")) {
                        carpet_params.depth = 7;
                        camera.zoom = 1.0f;
                        camera.target = (Vector2){WIDTH/2.0f, HEIGHT/2.0f};
                        camera_offset = 20.0f;
                        needs_update = true;
                    }
                    break;
                case STATE_TRIANGLE:
                    GuiLabel((Rectangle){20, 50, 200, 20}, TextFormat("Depth: %d", triangle_params.depth));
                    GuiSlider((Rectangle){20, 80, 200, 20}, NULL, NULL,
                        &triangle_params.depth, 0, (float) triangle_params.max_depth);
                    triangle_params.depth = (float) (int) triangle_params.depth;

                    if (GuiButton((Rectangle){20, 120, 110, 30}, "Reset")) {
                        triangle_params.depth = 9;
                        camera.zoom = 1.0f;
                        camera.target = (Vector2){WIDTH/2.0f, HEIGHT/2.0f};
                        camera_offset = 20.0f;
                    }
                    break;
                case STATE_MANDELBROT:
                    GuiLabel((Rectangle){20, 50, 200, 20}, TextFormat("Iterations: %d", (int) mandel_params.iterations));
                    if (GuiSlider((Rectangle){20, 80, 200, 20}, NULL, NULL,
                        &mandel_params.iterations, 0, 500)) {

                        mandel_params.iterations = (float)(int) mandel_params.iterations;
                        needs_update = true;
                    }

                    GuiLabel((Rectangle){20, 110, 200, 20}, TextFormat("Palette:"));
                    GuiLabel((Rectangle){20, 130, 200, 20}, TextFormat("Red factor: "));
                    if (GuiSlider((Rectangle){20, 150, 200, 20}, NULL, NULL,
                        &mandel_params.red, 0, 20)) {
                        needs_update = true;
                    }

                    GuiLabel((Rectangle){20, 170, 200, 20}, TextFormat("Green factor:"));
                    if (GuiSlider((Rectangle){20, 190, 200, 20}, NULL, NULL,
                        &mandel_params.green, 0, 20)) {
                        needs_update = true;
                    }

                    GuiLabel((Rectangle){20, 210, 200, 20}, TextFormat("Blue factor:"));
                    if (GuiSlider((Rectangle){20, 230, 200, 20}, NULL, NULL,
                        &mandel_params.blue, 0, 20)) {
                        needs_update = true;
                    }

                    if (GuiButton((Rectangle){20, 280, 110, 30}, "Reset")) {
                        mandel_params.iterations = 200;
                        mandel_params.max_iterations = 800;
                        mandel_params.zoom = 1.0f;
                        mandel_params.offset_x = 0.0f;
                        mandel_params.offset_y = 0.0f;
                        camera.target = (Vector2){WIDTH / 2.0f - 300.0f, HEIGHT / 2.0f};
                        camera.offset = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
                        camera.rotation = 0.0f;
                        camera.zoom = 1.0f;
                        mandel_params.red = 9.0f;
                        mandel_params.green = 15.f;
                        mandel_params.blue = 8.5f;
                        needs_update = true;
                    }
                    break;
            }

            if (GuiButton((Rectangle){20, 330, 110, 30}, "<- Gallery")) {
                state = STATE_GALLERY;
            }
        }

        DrawFPS(WIDTH - 90, 15);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}