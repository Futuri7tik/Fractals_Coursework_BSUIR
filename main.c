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
    FractalParameters fract_params;
    init_fractals_parameters(&fract_params);

    Camera2D camera = {0};
    camera.target = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
    camera.offset = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    float camera_offset = 20.0f;

    // === Главный цикл ===
    while (!WindowShouldClose()) {
        // === Обработка клавиш ===

        if (IsKeyPressed(KEY_TAB)) {
            GuiLoadStyleDefault();
            if (state == STATE_GALLERY)
                state = STATE_MENU;
            else
                if (state != STATE_MENU)
                    state = STATE_GALLERY;
        }

        if (state != STATE_GALLERY && state != STATE_MENU && state != STATE_SLIDESHOW) {
            float current_speed = 20.0f / camera.zoom;

            handle_movement(current_speed, &camera_offset, &camera, &needs_update);
        }

        // === Отрисовка ===
        BeginDrawing();
        ClearBackground(BLACK);

        if (state == STATE_MENU) {
            bool should_close = false;

            menu_gui(&state, &show_message_box, &should_close);
            if (should_close)
                break;
        }
        else
            if (state == STATE_GALLERY) {
                GuiLoadStyleDefault();
                ImageNode* head = NULL;

                gallery_gui(&state, &fract_params, &camera, &camera_offset, &head, &needs_update);
                GuiLoadStyleDefault();
            }
            else {
                switch (state) {
                    case STATE_TREE:
                        BeginMode2D(camera);
                        draw_tree(WIDTH/2.0f, HEIGHT - 220, 200, -PI/2.0f,
                             (int) fract_params.tree.depth, &fract_params.tree);
                        EndMode2D();
                        break;
                    case STATE_CARPET: {
                        BeginMode2D(camera);
                        if (needs_update) {
                            if (fract_params.carpet.texture.id > 0)
                                UnloadTexture(fract_params.carpet.texture);
                            fract_params.carpet.texture = render_carpet_to_texture(WIDTH, HEIGHT,
                                                      (int) fract_params.carpet.depth,
                                                      fract_params.carpet.start_length);
                            needs_update = false;
                        }

                        if (fract_params.carpet.texture.id > 0)
                            DrawTexture(fract_params.carpet.texture, 0, 0, WHITE);
                        EndMode2D();
                        break;
                    }
                    case STATE_TRIANGLE: {
                        BeginMode2D(camera);
                        float start_height = fract_params.triangle.start_length * sqrtf(3.0f) / 2.0f;
                        float x_start = (WIDTH - fract_params.triangle.start_length) / 2.0f;
                        float y_start = HEIGHT - (HEIGHT - start_height) / 2.0f;
                        draw_triangle_base(x_start, y_start, fract_params.triangle.start_length, YELLOW);
                        draw_sierpinski_triangle(x_start, y_start, fract_params.triangle.start_length,
                                               (int) fract_params.triangle.depth, &fract_params.triangle);
                        EndMode2D();
                        break;
                    }
                    case STATE_MANDELBROT: {
                        if (needs_update == true) {
                            if (fract_params.mandelbrot.texture.id > 0)
                                UnloadTexture(fract_params.mandelbrot.texture);
                            fract_params.mandelbrot.zoom = camera.zoom;
                            fract_params.mandelbrot.offset_x = (camera.target.x - WIDTH / 2.0f) * (4.0f / WIDTH);
                            fract_params.mandelbrot.offset_y = (camera.target.y - HEIGHT / 2.0f) * (4.0f / WIDTH);

                            fract_params.mandelbrot.texture = render_mandelbrot(WIDTH, HEIGHT,
                                                  fract_params.mandelbrot.zoom,
                                                  fract_params.mandelbrot.offset_x,
                                                  fract_params.mandelbrot.offset_y,
                                                  (int) fract_params.mandelbrot.iterations,
                                                  &fract_params.mandelbrot);
                            needs_update = false;
                        }

                        if (fract_params.mandelbrot.texture.id > 0)
                            DrawTexture(fract_params.mandelbrot.texture, 0, 0, WHITE);
                        break;
                    }
                    case STATE_JULIA: {
                        if (needs_update == true) {
                            if (fract_params.julia.texture.id > 0)
                                UnloadTexture(fract_params.julia.texture);

                            fract_params.julia.zoom = camera.zoom;
                            fract_params.julia.offset_x = (camera.target.x - WIDTH / 2.0f) * (4.0f / WIDTH);
                            fract_params.julia.offset_y = (camera.target.y - HEIGHT / 2.0f) * (4.0f / WIDTH);

                            fract_params.julia.texture = render_julia(WIDTH, HEIGHT,
                                             fract_params.julia.zoom,
                                             fract_params.julia.offset_x,
                                                  fract_params.julia.offset_y,
                                                  (int) fract_params.julia.iterations,
                                                  &fract_params.julia);
                            needs_update = false;
                        }

                        if (fract_params.julia.texture.id > 0)
                            DrawTexture(fract_params.julia.texture, 0, 0, WHITE);
                        break;
                    }
                    default:
                        break;
                }

                GuiPanel((Rectangle){10, 10, 280, 450}, "Controls");
                switch (state) {
                    case STATE_TREE:
                        tree_gui(&fract_params, &camera, &camera_offset);
                        break;
                    case STATE_CARPET:
                        carpet_gui(&fract_params, &camera, &camera_offset, &needs_update);
                        break;
                    case STATE_TRIANGLE:
                        triangle_gui(&fract_params, &camera, &camera_offset);
                        break;
                    case STATE_MANDELBROT:
                        mandelbrot_gui(&fract_params, &camera, &needs_update);
                        break;
                    case STATE_JULIA:
                        julia_gui(&fract_params, &camera, &needs_update);
                        break;
                    default:
                        break;
                }

                if (GuiButton((Rectangle){20, 410, 110, 30}, "<- Gallery")) {
                    state = STATE_GALLERY;
                }
                DrawFPS(WIDTH - 90, 15);
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}