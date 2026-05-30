#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"

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
    // включаем сглаживание MSAA4X для плавных линий. инициализируем окно
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(WIDTH, HEIGHT, "Fractal Gallery");

    // задаем начальные значения флагов и загружаем нужные изображения
    bool needs_update = true, was_updating = false, show_message_box = false;
    bool is_sorted = false;
    ImageNode* unsorted_head = malloc(sizeof(ImageNode));
    Texture2D font = LoadTexture("resources/font2.png"), dev = LoadTexture("resources/dev.png");

    SetTargetFPS(144);

    // инициализируем состояние приложения и параметры фракталов
    AppState state = STATE_MENU;
    FractalParameters fract_params;
    init_fractals_parameters(&fract_params);

    // добавляем параметры в стек
    clear_undo();
    push(&fract_params);

    // задаем состояние камеры
    Camera2D camera = {0};
    camera.target = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
    camera.offset = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // инициализируем состояние рандома
    AppState random_type = STATE_GALLERY;
    FractalParameters random_params;

    // задаем список
    ImageNode* head = NULL;
    load_pics(&head);

    // основной цикл приложения
    while (!WindowShouldClose()) {
        // начинаем отрисовку
        BeginDrawing();
        ClearBackground(BLACK);

        // логика отката к предыдущему состоянию
        if (IsKeyPressed(KEY_TAB)) {
            GuiLoadStyleDefault();
            clear_undo();
            if (state == STATE_GALLERY)
                state = STATE_MENU;
            else
                if (state != STATE_MENU && state != STATE_SLIDESHOW)
                    state = STATE_GALLERY;
                else
                    state = STATE_MENU;
        }

        // логика приложения в зависимости от состояния
        switch (state) {
            case STATE_MENU: {
                bool should_close = false;

                // отрисовываем меню
                menu_gui(&state, &show_message_box, &should_close, font, dev);
                if (should_close)
                    exit(0);

                random_type = STATE_GALLERY;
                break;
            }
            case STATE_GALLERY: {
                // отрисовываем gui галереи
                gallery_gui(&state, &fract_params, &camera, &head, &needs_update);
                GuiLoadStyleDefault();
                GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

                // вызов сортировки галереи
                if (GuiButton((Rectangle) {1690, 130, 200, 50},
                    (is_sorted == false) ? "Sort by name" : "Undo")) {
                    if (is_sorted == false) {
                        copy_list(unsorted_head, head);

                        head->next = merge_sort(head->next);
                        update_gallery_positions(head);
                    }
                    else
                        copy_list(head, unsorted_head);

                    is_sorted = !is_sorted;
                }

                // кнопка выхода
                if (GuiButton((Rectangle) {1690, 190, 200, 50}, "Exit")) {
                    exit(0);
                }

                GuiLoadStyleDefault();
                random_type = STATE_GALLERY;
                break;
            }
            case STATE_SLIDESHOW: {
                // отрисовка слайд-шоу
                show_slideshow(head, &state);

                break;
            }
            case STATE_RANDOM: {
                GuiLoadStyleDefault();

                // инициализация параметров случайного фрактала
                if (random_type == STATE_GALLERY) {
                    init_random_config(&random_params, &camera,&random_type);
                    camera.zoom = 1.0f;
                    needs_update = true;
                }

                // обработка движения по фракталу
                float current_speed = 20.0f / camera.zoom;
                handle_movement(current_speed, &camera, &needs_update);
                render_fractals(&camera, &random_type, &random_params, &needs_update);

                // отрисовка gui фрактала
                GuiPanel((Rectangle){10, 10, 250, 450}, "Controls");
                DrawText("Random Fractal", 165, 17, 10, DARKGRAY);
                render_fractal_gui(&camera, &random_params, &random_type, &needs_update);

                // возврат к галерее по нажатию на кнопку
                if (GuiButton((Rectangle){20, 420, 110, 30}, "<- Gallery"))
                    state = STATE_GALLERY;

                // сохранение изображения
                if (GuiButton((Rectangle){140, 380, 110, 30}, "Save Image"))
                    save_image(state, random_type, &fract_params, &random_params, &needs_update);

                // откат через стек по нажатию на кнопку
                if (GuiButton((Rectangle){140, 420, 110, 30}, "Undo")) {
                    if (pop(&random_params)) {
                        needs_update = true;
                        was_updating = true;
                    }
                }

                // отрисовка количества кадров в секунду
                DrawFPS(WIDTH - 90, 15);

                break;
            }
            default: {
                GuiLoadStyleDefault();

                // обработка движения по фракталу
                float current_speed = 20.0f / camera.zoom;
                handle_movement(current_speed, &camera, &needs_update);
                render_fractals(&camera, &state, &fract_params, &needs_update);

                // отрисовка gui фракталов
                GuiPanel((Rectangle){10, 10, 250, 450}, "Controls");
                render_fractal_gui(&camera, &fract_params, &state, &needs_update);

                // возврат к галерее по нажатию на кнопку
                if (GuiButton((Rectangle){20, 420, 110, 30}, "<- Gallery"))
                    state = STATE_GALLERY;

                // сохранение изображения
                if (GuiButton((Rectangle){140, 380, 110, 30}, "Save Image"))
                    save_image(state, random_type, &fract_params, &random_params, &needs_update);

                // откат через стек по нажатию на кнопку
                if (GuiButton((Rectangle){140, 420, 110, 30}, "Undo")) {
                    if (pop(&fract_params)) {
                        needs_update = true;
                        was_updating = true;
                    }
                }

                DrawFPS(WIDTH - 90, 15);

                random_type = STATE_GALLERY;
                break;
            }
        }

        // обновление стека
        if (needs_update && !was_updating) {
            if (state == STATE_RANDOM)
                push(&random_params);
            else
                push(&fract_params);
        }

        was_updating = needs_update;
        EndDrawing();
    }

    // выгрузка загруженных текстур. очистка выделенной памяти
    UnloadTexture(font);
    UnloadTexture(dev);
    free(unsorted_head);
    free_list(head);
    rewrite_fractal_parameters(&fract_params);

    CloseWindow();
    return 0;
}