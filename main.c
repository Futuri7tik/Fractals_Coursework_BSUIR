#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "win32_save.h"

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
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(WIDTH, HEIGHT, "Fractal Gallery");
    bool needs_update = true;
    static bool was_updating = false;
    bool show_message_box = false;

    SetTargetFPS(144);

    AppState state = STATE_MENU;
    FractalParameters fract_params;
    init_fractals_parameters(&fract_params);

    clear_undo();
    push(&fract_params);

    Camera2D camera = {0};
    camera.target = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
    camera.offset = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    AppState random_type = STATE_GALLERY;
    FractalParameters random_params;

    ImageNode* head = NULL;
    load_pics(&head);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_TAB)) {
            GuiLoadStyleDefault();
            clear_undo();
            if (state == STATE_GALLERY)
                state = STATE_MENU;
            else
                if (state != STATE_MENU)
                    state = STATE_GALLERY;
        }

        if (state == STATE_RANDOM && random_type == STATE_GALLERY) {
            init_random_config(&random_params, &camera,&random_type);
            camera.zoom = 1.0f;
            needs_update = true;
        }

        if (state != STATE_RANDOM) {
            random_type = STATE_GALLERY;
        }

        if (state != STATE_GALLERY && state != STATE_MENU && state != STATE_SLIDESHOW) {
            float current_speed = 20.0f / camera.zoom;
            handle_movement(current_speed, &camera, &needs_update);
        }

        if (needs_update && !was_updating) {
            if (state == STATE_RANDOM)
                push(&random_params);
            else
                push(&fract_params);
        }
        was_updating = needs_update;

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

                gallery_gui(&state, &fract_params, &camera, &head, &needs_update);
                GuiLoadStyleDefault();
            }
            else {
                if (state != STATE_RANDOM) {
                    render_fractals(&camera, &state, &fract_params, &needs_update);
                }
                else
                    render_fractals(&camera, &random_type, &random_params, &needs_update);

                GuiPanel((Rectangle){10, 10, 250, 450}, "Controls");
                if (state != STATE_RANDOM) {
                    render_fractal_gui(&camera, &fract_params, &state, &needs_update);
                }
                else {
                    DrawText("Random Fractal", 165, 17, 10, DARKGRAY);
                    render_fractal_gui(&camera, &random_params, &random_type, &needs_update);
                }

                if (GuiButton((Rectangle){20, 420, 110, 30}, "<- Gallery")) {
                    state = STATE_GALLERY;
                }

                if (GuiButton((Rectangle){140, 380, 110, 30}, "Save Image")) {
                    save_image(state, random_type, &fract_params, &random_params);
                }

                if (GuiButton((Rectangle){140, 420, 110, 30}, "Undo")) {
                    if (state != STATE_RANDOM) {
                        if (pop(&fract_params)) {
                            needs_update = true;
                            was_updating = true;
                        }
                    }
                    else
                        if (pop(&random_params)) {
                            needs_update = true;
                            was_updating = true;
                        }
                }

                DrawFPS(WIDTH - 90, 15);
            }

        EndDrawing();
    }

    rewrite_fractal_parameters(&fract_params);
    CloseWindow();
    return 0;
}