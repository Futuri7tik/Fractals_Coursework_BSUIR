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
    bool show_message_box = false;

    SetTargetFPS(144);

    AppState state = STATE_MENU;

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

    // === Главный цикл ===
    while (!WindowShouldClose()) {
        // === Обработка клавиш ===
        // Навигация камеры
        if (IsKeyPressed(KEY_TAB) && state != STATE_MENU) {
            state = STATE_GALLERY;
        }

        if (state != STATE_GALLERY && state != STATE_MENU) {
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

        if (state == STATE_MENU) {
            DrawRectangleGradientV(0, 0, WIDTH, HEIGHT,
                    (Color){255, 255, 255, 255}, (Color){106, 85, 242, 255});
            int fontSize = 80, button_text = 30;
            char* text= "FRACTAL GALLERY";
            int textWidth = MeasureText(text, fontSize);
            int x_coord = (WIDTH - textWidth) / 2, y_coord = HEIGHT/2 - 350;
            DrawText(text, x_coord, y_coord, fontSize, BLACK);

            int gallery_button = GuiButton((Rectangle){WIDTH / 2.0f - 200, HEIGHT / 2.0f - 200, 400, 60}, "GALLERY");
            GuiSetStyle(gallery_button, TEXT_SIZE, button_text);

            if (gallery_button)
                state = STATE_GALLERY;

            int about_button = GuiButton((Rectangle){WIDTH / 2.0f - 200, HEIGHT / 2.0f - 130, 400, 60}, "ABOUT");
            if (about_button)
                show_message_box = true;

            if (show_message_box) {
                GuiLoadStyleDefault();
                int result = GuiMessageBox(
                (Rectangle){WIDTH/2.0f - 300, HEIGHT/2.0f - 200, 600, 400},
                    "About",
                "Fractal Gallery v1.0\n\n"
                     "Interactive Fractal Explorer\n\n"
                     "Features:\n"
                     "- Pythagoras Tree\n"
                     "- Sierpinski Carpet\n"
                     "- Sierpinski Triangle\n"
                     "- Mandelbrot Set\n\n"
                     "Controls:\n"
                     "- Mouse Wheel: Zoom\n"
                     "- Arrow Keys: Pan\n"
                     "- Sliders: Fractal parameters"
                     "- Tab - go back to gallery\n"
                     "- ESC: Exit",
                "OK");

                if (result >= 0) {
                    show_message_box = false;
                    GuiSetStyle(gallery_button, TEXT_SIZE, button_text);
                }

            }

            if (GuiButton((Rectangle){WIDTH / 2.0f - 200, HEIGHT / 2.0f - 60, 400, 60}, "EXIT")) {
                break;
            }
        }
        else

        if (state == STATE_GALLERY) {
            GuiLoadStyleDefault();
            GuiPanel((Rectangle){0, 10, WIDTH, HEIGHT - 10}, "Fractal Gallery");
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
        }
        else {
            switch (state) {
                case STATE_TREE:
                    BeginMode2D(camera);
                    draw_tree(WIDTH/2.0f, HEIGHT - 220, 200, -PI/2.0f,
                         (int) tree_params.depth, &tree_params);
                    EndMode2D();
                    break;
                case STATE_CARPET: {
                    BeginMode2D(camera);
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
                    EndMode2D();
                    break;
                }
                case STATE_TRIANGLE: {
                    BeginMode2D(camera);
                    float start_height = triangle_params.start_length * sqrtf(3.0f) / 2.0f;
                    float x_start = (WIDTH - triangle_params.start_length) / 2.0f;
                    float y_start = HEIGHT - (HEIGHT - start_height) / 2.0f;
                    draw_triangle_base(x_start, y_start, triangle_params.start_length, YELLOW);
                    draw_sierpinski_triangle(x_start, y_start, triangle_params.start_length,
                                           (int) triangle_params.depth, &triangle_params);
                    EndMode2D();
                    break;
                }
                case STATE_MANDELBROT: {
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
                    break;
                }
                default:
                    break;
            }

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
                        init_tree_parameters(&tree_params);
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
                        init_carpet_parameters(&carpet_params);
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
                        init_triangle_parameters(&triangle_params);
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
                        init_mandelbrot_parameters(&mandel_params);
                        camera.target = (Vector2){WIDTH / 2.0f - 300.0f, HEIGHT / 2.0f};
                        camera.offset = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
                        camera.rotation = 0.0f;
                        camera.zoom = 1.0f;
                        needs_update = true;
                    }
                    break;
                default:
                    break;
            }

            if (GuiButton((Rectangle){20, 330, 110, 30}, "<- Gallery")) {
                state = STATE_GALLERY;
            }
            DrawFPS(WIDTH - 90, 15);
        }


        EndDrawing();
    }

    CloseWindow();
    return 0;
}