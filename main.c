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
    AppState random_type = STATE_GALLERY;
    FractalParameters random_params;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_TAB)) {
            GuiLoadStyleDefault();
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

                gallery_gui(&state, &fract_params, &camera, &head, &needs_update);
                GuiLoadStyleDefault();
            }
            else {
                if (state != STATE_RANDOM) {
                    render_fractals(&camera, &state, &fract_params, &needs_update);
                }
                else
                    render_fractals(&camera, &random_type, &random_params, &needs_update);

                GuiPanel((Rectangle){10, 10, 280, 450}, "Controls");
                if (state != STATE_RANDOM) {
                    render_fractal_gui(&camera, &fract_params, &state, &needs_update);
                }
                else {
                    DrawText("Random Fractal", 200, 17, 10, DARKGRAY);
                    render_fractal_gui(&camera, &random_params, &random_type, &needs_update);
                }

                if (GuiButton((Rectangle){20, 420, 110, 30}, "<- Gallery")) {
                    state = STATE_GALLERY;
                }
                DrawFPS(WIDTH - 90, 15);
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}