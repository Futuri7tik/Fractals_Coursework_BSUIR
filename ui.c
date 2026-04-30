#include <math.h>
#include <raygui.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "functions.h"
#include "win32_save.h"

void menu_gui(AppState* state, bool* show_msg_box, bool* should_close) {
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
        *state = STATE_GALLERY;

    if (GuiButton((Rectangle){WIDTH / 2.0f - 200, HEIGHT / 2.0f - 130, 400, 60}, "SLIDESHOW")) {
        *state = STATE_SLIDESHOW;
    }

    int about_button = GuiButton((Rectangle){WIDTH / 2.0f - 200, HEIGHT / 2.0f - 60, 400, 60}, "ABOUT");
    if (about_button)
        *show_msg_box = true;

    if (GuiButton((Rectangle){WIDTH / 2.0f - 200, HEIGHT / 2.0f + 10, 400, 60}, "EXIT")) {
        *should_close = true;
    }

    if (*show_msg_box) {
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
             "- Mandelbrot Set\n"
             "- Julia Set\n"
             "- Circle Fractal\n"
             "- Barnsley Fern\n"
             "- Newtons Fractal\n\n"
             "Controls:\n"
             "- Mouse Wheel: Zoom\n"
             "- Arrow Keys: Pan\n"
             "- Sliders: Fractal parameters\n"
             "- Tab: go back to gallery from render or to menu from gallery\n"
             "- ESC: Exit",
        "OK");

        if (result >= 0) {
            *show_msg_box = false;
            GuiSetStyle(gallery_button, TEXT_SIZE, button_text);
        }
    }
}

void handle_movement(float speed, Camera2D* cam, bool* update) {
    // Вычисляем вектор движения: (Вправо - Влево, Вниз - Вверх)
    int moveX = IsKeyPressedRepeat(KEY_RIGHT) - IsKeyPressedRepeat(KEY_LEFT);
    int moveY = IsKeyPressedRepeat(KEY_DOWN) - IsKeyPressedRepeat(KEY_UP);

    if (moveX != 0 || moveY != 0) {
        cam->target.x += (float) moveX * speed;
        cam->target.y += (float) moveY * speed;
        *update = true;
    }

    float wheel = GetMouseWheelMove();
    if (wheel > 0) {
        cam->zoom *= 1.05f;
        *update = true;
    }
    else
        if (wheel < 0) {
            cam->zoom *= 0.95f;
            *update = true;
        }
}

void load_pics(ImageNode** head) {
    char* fractal_names[] = {"Mandelbrot Set", "Pythagorean Tree","Sierpinski Carpet", "Sierpinski Triangle",
        "Julia Set", "Circle Fractal","Barnsley Fern", "Newtons Fractal", "Random Fractal"};
    char* image_names[] = {"mandelbrot.png", "tree.png", "carpet.png", "triangle.png", "julia.png",
        "circle.png","fern.png","newton.png", "random.png"};

    size_t size = sizeof(fractal_names) / sizeof(fractal_names[0]);
    Rectangle *fields = calloc(size, sizeof(Rectangle));

    if (*head == NULL) {
        load_gallery(head, fractal_names, image_names, size, fields);
    }

    free(fields);
}

void gallery_gui(AppState *state, FractalParameters *params, Camera2D *cam, ImageNode **head_img,
                 bool *update) {
    GuiPanel((Rectangle){0, 10, WIDTH, HEIGHT - 10}, "Fractal Gallery");
    cam->zoom = 1.0f;
    cam->target = (Vector2){WIDTH/2.0f, HEIGHT/2.0f};

    draw_pics(*head_img);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x00000000);
    ImageNode* current = (*head_img)->next;

    while (current != NULL) {
        if (GuiButton(current->field, "")) {
            if (current->target_state == STATE_MANDELBROT) {
                *update = true;
                params->mandelbrot.zoom = cam->zoom;
                cam->target = (Vector2){WIDTH / 2.0f - 300.0f, HEIGHT / 2.0f};
                params->mandelbrot.offset_x = (cam->target.x - WIDTH / 2.0f) / WIDTH;
                params->mandelbrot.offset_y = (cam->target.y - HEIGHT / 2.0f) / HEIGHT;
            }

            *state = current->target_state;
            *update = true;
        }
        current = current->next;
    }
}

void tree_gui(FractalParameters* params, Camera2D* cam, bool *update) {
    GuiLabel((Rectangle){20, 50, 200, 20}, TextFormat("Depth: %d", (int) params->tree.depth));

    if (GuiSlider((Rectangle){20, 80, 200, 20}, NULL, NULL,
        &params->tree.depth, 1,(float)params->tree.max_depth)) {
        *update = true;
    }
    params->tree.depth = (float) (int)params->tree.depth;

    GuiLabel((Rectangle){20, 110, 200, 20}, TextFormat("Angle: %.1f°", params->tree.angle_degrees));
    if (GuiSlider((Rectangle){20, 130, 200, 20}, NULL, NULL,
        &params->tree.angle_degrees, 0, 90)) {
        *update = true;
    }

    params->tree.angle = params->tree.angle_degrees * DEG2RAD;

    GuiLabel((Rectangle){20, 150, 200, 20}, TextFormat("Length Factor: %.2f", params->tree.length_factor));
    if (GuiSlider((Rectangle){20, 170, 200, 20}, NULL, NULL,
        &params->tree.length_factor, 0, 0.9f)) {
        *update = true;
    }

    if (GuiButton((Rectangle){20, 380, 110, 30}, "Reset")) {
        init_default_tree_parameters(&params->tree);
        *update = true;
        cam->zoom = 1.0f;
        cam->target = (Vector2){WIDTH/2.0f, HEIGHT/2.0f};
    }
}

void carpet_gui(FractalParameters* params, Camera2D* cam, bool* update) {
    GuiLabel((Rectangle){20, 50, 200, 20}, TextFormat("Depth: %d", (int) params->carpet.depth));
    if (GuiSlider((Rectangle){20, 80, 200, 20}, NULL, NULL,
        &params->carpet.depth, 0, (float) params->carpet.max_depth)) {
        *update = true;
    }

    params->carpet.depth = (float) (int) params->carpet.depth;

    GuiLabel((Rectangle){20, 110, 200, 20}, TextFormat("Palette:"));
    GuiLabel((Rectangle){20, 130, 200, 20}, TextFormat("Red factor: %d", (int) params->carpet.red));
    if (GuiSlider((Rectangle){20, 150, 200, 20}, NULL, NULL,
                  &params->carpet.red, 0, 255)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 170, 200, 20}, TextFormat("Green factor: %d", (int) params->carpet.green));
    if (GuiSlider((Rectangle){20, 190, 200, 20}, NULL, NULL,
                  &params->carpet.green, 0, 255)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 210, 200, 20}, TextFormat("Blue factor: %d", (int) params->carpet.blue));
    if (GuiSlider((Rectangle){20, 230, 200, 20}, NULL, NULL,
        &params->carpet.blue, 0, 255)) {
        *update = true;
    }

    if (GuiButton((Rectangle){20, 380, 110, 30}, "Reset")) {
        init_default_carpet_parameters(&params->carpet);
        cam->zoom = 1.0f;
        cam->target = (Vector2){WIDTH/2.0f, HEIGHT/2.0f};
        *update = true;
    }
}

void triangle_gui(FractalParameters* params, Camera2D* cam, bool* update) {
    GuiLabel((Rectangle){20, 50, 200, 20}, TextFormat("Depth: %d", (int) params->triangle.depth));
    if (GuiSlider((Rectangle){20, 80, 200, 20}, NULL, NULL,
        &params->triangle.depth, 0, (float) params->triangle.max_depth)) {
        *update = true;
    }
    params->triangle.depth = (float) (int) params->triangle.depth;

    GuiLabel((Rectangle){20, 110, 200, 20}, TextFormat("Palette:"));

    GuiLabel((Rectangle){20, 130, 200, 20}, TextFormat("Red component: %d", (int) params->triangle.red));
    if (GuiSlider((Rectangle){20, 150, 200, 20}, NULL, NULL,
        &params->triangle.red, 0, 255)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 170, 200, 20}, TextFormat("Green component: %d:", (int) params->triangle.green));
    if (GuiSlider((Rectangle){20, 190, 200, 20}, NULL, NULL,
        &params->triangle.green, 0, 255)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 210, 200, 20}, TextFormat("Blue component: %d", (int) params->triangle.blue));
    if (GuiSlider((Rectangle){20, 230, 200, 20}, NULL, NULL,
                  &params->triangle.blue, 0, 255)) {
        *update = true;
    }

    if (GuiButton((Rectangle){20, 380, 110, 30}, "Reset")) {
        init_default_triangle_parameters(&params->triangle);
        *update = true;
        cam->zoom = 1.0f;
        cam->target = (Vector2){WIDTH/2.0f, HEIGHT/2.0f};
    }
}

void mandelbrot_gui(FractalParameters* params, Camera2D* cam, bool* update) {
    GuiLabel((Rectangle){20, 50, 200, 20}, TextFormat("Iterations: %d", (int) params->mandelbrot.iterations));
    if (GuiSlider((Rectangle){20, 80, 200, 20}, NULL, NULL,
                  &params->mandelbrot.iterations, 0, (float) params->mandelbrot.max_iterations)) {
        params->mandelbrot.iterations = (float) (int) params->mandelbrot.iterations;
        *update = true;
    }

    GuiLabel((Rectangle){20, 110, 200, 20}, TextFormat("Palette:"));
    GuiLabel((Rectangle){20, 130, 200, 20}, TextFormat("Red factor: %d", (int) params->mandelbrot.red));
    if (GuiSlider((Rectangle){20, 150, 200, 20}, NULL, NULL,
                  &params->mandelbrot.red, 0, 20)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 170, 200, 20}, TextFormat("Green factor: %d", (int) params->mandelbrot.green));
    if (GuiSlider((Rectangle){20, 190, 200, 20}, NULL, NULL,
                  &params->mandelbrot.green, 0, 20)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 210, 200, 20}, TextFormat("Blue factor: %d", (int) params->mandelbrot.blue));
    if (GuiSlider((Rectangle){20, 230, 200, 20}, NULL, NULL,
                  &params->mandelbrot.blue, 0, 20)) {
        *update = true;
    }

    if (GuiButton((Rectangle){20, 380, 110, 30}, "Reset")) {
        init_default_mandelbrot_parameters(&params->mandelbrot);
        cam->target = (Vector2){WIDTH / 2.0f - 300.0f, HEIGHT / 2.0f};
        cam->offset = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
        cam->zoom = 1.0f;
        *update = true;
    }
}

void julia_gui(FractalParameters* params, Camera2D* cam, bool* update) {
    GuiLabel((Rectangle){20, 50, 200, 20}, TextFormat("Iterations: %d", (int) params->julia.iterations));
    if (GuiSlider((Rectangle){20, 80, 200, 20}, NULL, NULL,
                  &params->julia.iterations, 0, (float) params->julia.max_iterations)) {
        params->julia.iterations = (float) (int) params->julia.iterations;
        *update = true;
    }

    GuiLabel((Rectangle){20, 110, 200, 20}, TextFormat("Palette:"));
    GuiLabel((Rectangle){20, 130, 200, 20}, TextFormat("Red factor: %d", (int) params->julia.red));
    if (GuiSlider((Rectangle){20, 150, 200, 20}, NULL, NULL,
                  &params->julia.red, 0, 20)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 170, 200, 20}, TextFormat("Green factor %d:", (int) params->julia.green));
    if (GuiSlider((Rectangle){20, 190, 200, 20}, NULL, NULL,
                  &params->julia.green, 0, 20)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 210, 200, 20}, TextFormat("Blue factor: %d", (int) params->julia.blue));
    if (GuiSlider((Rectangle){20, 230, 200, 20}, NULL, NULL,
                  &params->julia.blue, 0, 20)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 260, 200, 20}, TextFormat("Constant c: "));
    GuiLabel((Rectangle){20, 280, 200, 20}, TextFormat("Real part: %.3f", params->julia.re_c));
    if (GuiSlider((Rectangle){20, 300, 200, 20}, NULL, NULL,
                  &params->julia.re_c, -1, 1)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 320, 200, 20}, TextFormat("Complex part: %.3f", params->julia.im_c));
    if (GuiSlider((Rectangle){20, 340, 200, 20}, NULL, NULL,
                  &params->julia.im_c, -1, 1)) {
        *update = true;
    }

    if (GuiButton((Rectangle){20, 380, 110, 30}, "Reset")) {
        init_default_julia_parameters(&params->julia);
        cam->target = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
        cam->offset = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
        cam->zoom = 1.0f;
        *update = true;
    }
}

void circle_gui(FractalParameters* params, Camera2D* cam, bool* update) {
    GuiLabel((Rectangle){20, 50, 200, 20}, TextFormat("Depth: %d", (int) params->circle.depth));
    if (GuiSlider((Rectangle){20, 80, 200, 20}, NULL, NULL,
        &params->circle.depth, 0, (float) params->circle.max_depth)) {
        *update = true;
    }
    params->circle.depth = (float) (int) params->circle.depth;

    GuiLabel((Rectangle){20, 110, 200, 20}, TextFormat("Palette:"));

    GuiLabel((Rectangle){20, 130, 200, 20}, TextFormat("Red component: %d", (int) params->circle.red));
    if (GuiSlider((Rectangle){20, 150, 200, 20}, NULL, NULL,
        &params->circle.red, 0, 255)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 170, 200, 20}, TextFormat("Green component: %d:", (int) params->circle.green));
    if (GuiSlider((Rectangle){20, 190, 200, 20}, NULL, NULL,
        &params->circle.green, 0, 255)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 210, 200, 20}, TextFormat("Blue component: %d", (int) params->circle.blue));
    if (GuiSlider((Rectangle){20, 230, 200, 20}, NULL, NULL,
                  &params->circle.blue, 0, 255)) {
        *update = true;
    }

    if (GuiButton((Rectangle){20, 380, 110, 30}, "Reset")) {
        init_default_circle_parameters(&params->circle);
        cam->zoom = 1.0f;
        cam->target = (Vector2){WIDTH/2.0f, HEIGHT/2.0f};
    }
}

void fern_gui(FractalParameters* params, Camera2D* cam, bool* update) {
    GuiLabel((Rectangle){20, 50, 200, 20}, TextFormat("Iterations: %d", (int) params->fern.iterations));
    if (GuiSlider((Rectangle){20, 80, 200, 20}, NULL, NULL,
                  &params->fern.iterations, 0, (float) params->fern.max_iterations)) {
        params->fern.iterations = (float) (int) params->fern.iterations;
        *update = true;
    }

    GuiLabel((Rectangle){20, 110, 200, 20}, TextFormat("Probabilities:"));

    GuiLabel((Rectangle){20, 130, 200, 20}, TextFormat("First probability: %d%%", (int) params->fern.prob1));
    if (GuiSlider((Rectangle){20, 150, 200, 20}, NULL, NULL,
        &params->fern.prob1, 0, 99)) {
        params->fern.prob1 = (float) (int) params->fern.prob1;
        *update = true;
    };

    GuiLabel((Rectangle){20, 170, 200, 20}, TextFormat("Second probability: %d%%", (int) params->fern.prob2));
    if (GuiSlider((Rectangle){20, 190, 200, 20}, NULL, NULL,
        &params->fern.prob2, 0, 99)) {
        params->fern.prob2 = (float) (int) params->fern.prob2;
        *update = true;
    };

    GuiLabel((Rectangle){20, 210, 200, 20}, TextFormat("Third probability: %d%%", (int) params->fern.prob3));
    if (GuiSlider((Rectangle){20, 230, 200, 20}, NULL, NULL,
        &params->fern.prob3, 0, 99)) {
        params->fern.prob3 = (float) (int) params->fern.prob3;
        *update = true;
    };

    GuiLabel((Rectangle){20, 250, 200, 20}, TextFormat("Fourth probability: %d%%", (int) params->fern.prob4));
    if (GuiSlider((Rectangle){20, 270, 200, 20}, NULL, NULL,
        &params->fern.prob4, 0, 99)) {
        params->fern.prob4 = (float) (int) params->fern.prob4;
        *update = true;
    };

    if (GuiButton((Rectangle){20, 380, 110, 30}, "Reset")) {
        init_default_fern_parameters(&params->fern);
        cam->zoom = 1.0f;
        cam->target = (Vector2){WIDTH/2.0f, HEIGHT/2.0f};
        *update = true;
    }
}

void newton_gui(FractalParameters* params, Camera2D* cam, bool* update) {
    GuiLabel((Rectangle){20, 50, 200, 20}, TextFormat("Iterations: %d", (int) params->newton.iterations));
    if (GuiSlider((Rectangle){20, 80, 200, 20}, NULL, NULL,
                  &params->newton.iterations, 0, (float) params->newton.max_iterations)) {
        params->newton.iterations = (float) (int) params->newton.iterations;
        *update = true;
    }

    GuiLabel((Rectangle){20, 110, 200, 20}, TextFormat("Palette:"));
    GuiLabel((Rectangle){20, 130, 200, 20}, TextFormat("Red factor: %d", (int) params->newton.red));
    if (GuiSlider((Rectangle){20, 150, 200, 20}, NULL, NULL,
                  &params->newton.red, 0, 255)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 170, 200, 20}, TextFormat("Green factor %d:", (int) params->newton.green));
    if (GuiSlider((Rectangle){20, 190, 200, 20}, NULL, NULL,
                  &params->newton.green, 0, 255)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 210, 200, 20}, TextFormat("Blue factor: %d", (int) params->newton.blue));
    if (GuiSlider((Rectangle){20, 230, 200, 20}, NULL, NULL,
                  &params->newton.blue, 0, 255)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 250, 200, 20}, TextFormat("Gradient:"));
    GuiLabel((Rectangle){20, 265, 200, 20}, TextFormat("Red factor: %d", (int) params->newton.gradient_r));
    if (GuiSlider((Rectangle){20, 285, 200, 15}, NULL, NULL,
                  &params->newton.gradient_r, 0, 255)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 300, 200, 20}, TextFormat("Green factor %d:", (int) params->newton.gradient_g));
    if (GuiSlider((Rectangle){20, 320, 200, 15}, NULL, NULL,
                  &params->newton.gradient_g, 0, 255)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 335, 200, 20}, TextFormat("Blue factor: %d", (int) params->newton.gradient_b));
    if (GuiSlider((Rectangle){20, 355, 200, 15}, NULL, NULL,
                  &params->newton.gradient_b, 0, 255)) {
        *update = true;
    }

    if (GuiButton((Rectangle){20, 380, 110, 30}, "Reset")) {
        init_default_newton_parameters(&params->newton);
        cam->target = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
        cam->offset = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
        cam->zoom = 1.0f;
        *update = true;
    }
}

ImageNode* create_image_node(char* fract_name, char* img_name, Rectangle field, Texture2D texture, AppState state) {
    ImageNode* node = malloc(sizeof(ImageNode));
    node->img_name = img_name;
    node->fract_name = fract_name;
    node->field = field;
    node->target_state = state;
    node->texture = texture;
    node->next = NULL;
    return node;
}

void load_gallery(ImageNode** head, char* fract_names[], char* img_names[], const size_t size, Rectangle* img_fields)
{
    *head = create_image_node("", "", (Rectangle) {0,0,0,0}, (Texture2D) {0}, STATE_MENU);
    ImageNode* temp = *head;
    AppState state = STATE_MANDELBROT;

    for (size_t i = 0; i < size; ++i) {
        const int img_height = 230, img_width = 280;
        const float y_start = 100, x_start = 150, x_indent = 350, y_indent = 300;

        Image temp_img = LoadImage(img_names[i]);
        ImageResize(&temp_img, img_width, img_height);
        const Rectangle field = (Rectangle) {x_start + (float) (i % 5) * x_indent, y_start + (int) (i / 5) * y_indent, img_width, img_height};
        img_fields[i] = field;
        ImageNode* new_node = create_image_node(fract_names[i], img_names[i],
            field, LoadTextureFromImage(temp_img), state);
        UnloadImage(temp_img);

        state += 1;
        temp->next = new_node;
        temp = temp->next;
    }
}

void draw_pics(ImageNode* head) {
    const ImageNode* temp = head;

    while (temp->next != NULL) {
        const int font_size = 24;

        temp = temp->next;
        const char* text = temp->fract_name;
        const int textWidth = MeasureText(text, font_size);

        DrawText(text, (temp->field.width - textWidth) / 2 + temp->field.x, (int) temp->field.y - 25, font_size, GRAY);
        DrawTexture(temp->texture, (int) temp->field.x, (int) temp->field.y, WHITE);
    }
}

void render_fractals(const Camera2D* cam, const AppState* state, FractalParameters* params, bool* update) {
    switch (*state) {
        case STATE_TREE:
            BeginMode2D(*cam);
            draw_tree(WIDTH/2.0f, HEIGHT - 220, 200, -PI/2.0f,
                 (int) params->tree.depth, &params->tree);
            EndMode2D();
            *update = false;
            break;
        case STATE_CARPET: {
            BeginMode2D(*cam);
            if (*update) {
                render_carpet(
                (int) params->carpet.depth, params->carpet.start_length,
                (Color){params->carpet.red, params->carpet.green, params->carpet.blue, 255},
                &params->carpet);

                *update = false;
            }

            DrawTexture(params->carpet.texture, 0, 0, WHITE);
            EndMode2D();
            break;
        }
        case STATE_TRIANGLE: {
            BeginMode2D(*cam);
            draw_triangle_base(params->triangle.x_start, params->triangle.y_start,
                params->triangle.start_length,
                (Color){params->triangle.red, params->triangle.green, params->triangle.blue, 255});
            draw_sierpinski_triangle(params->triangle.x_start, params->triangle.y_start, params->triangle.start_length,
                                   (int) params->triangle.depth, &params->triangle);
            EndMode2D();
            *update = false;
            break;
        }
        case STATE_MANDELBROT: {
            if (*update == true) {
                params->mandelbrot.zoom = cam->zoom;
                params->mandelbrot.offset_x = (cam->target.x - WIDTH / 2.0f) * (4.0f / WIDTH);
                params->mandelbrot.offset_y = (cam->target.y - HEIGHT / 2.0f) * (4.0f / WIDTH);

                render_mandelbrot(params->mandelbrot.zoom, params->mandelbrot.offset_x,params->mandelbrot.offset_y,
                (int)params->mandelbrot.iterations, &params->mandelbrot);

                *update = false;
            }

            DrawTexture(params->mandelbrot.texture, 0, 0, WHITE);
            break;
        }
        case STATE_JULIA: {
            if (*update == true) {
                params->julia.zoom = cam->zoom;
                params->julia.offset_x = (cam->target.x - WIDTH / 2.0f) * (4.0f / WIDTH);
                params->julia.offset_y = (cam->target.y - HEIGHT / 2.0f) * (4.0f / WIDTH);

                render_julia(params->julia.zoom, params->julia.offset_x,
                params->julia.offset_y, (int) params->julia.iterations, &params->julia);
                *update = false;
            }

            DrawTexture(params->julia.texture, 0, 0, WHITE);
            break;
        }
        case STATE_CIRCLE: {
            BeginMode2D(*cam);
            DrawCircleLinesV(params->circle.center, params->circle.radius,
                (Color) {params->circle.red, params->circle.green, params->circle.blue, 255});
            draw_circle_fractal(params->circle.center.x, params->circle.center.y, params->circle.radius / 3.0f,
                (int) params->circle.depth, (Color) {params->circle.red, params->circle.green, params->circle.blue, 255});
            EndMode2D();
            *update = false;
            break;
        }
        case STATE_FERN: {
            BeginMode2D(*cam);
            if (*update) {
                if (params->fern.texture.id > 0)
                    UnloadTexture(params->fern.texture);

                params->fern.texture = render_barnsley_fern((Vector2) {0, 0},(int) params->fern.iterations, params->fern);
                *update = false;
            }

            if (params->fern.texture.id > 0)
                DrawTexture(params->fern.texture, 0, 0, WHITE);

            EndMode2D();
            break;
        }
        case STATE_NEWTON: {
            if (*update == true) {
                params->newton.zoom = cam->zoom;
                params->newton.offset_x = (cam->target.x - WIDTH / 2.0f) * (4.0f / WIDTH);
                params->newton.offset_y = (cam->target.y - HEIGHT / 2.0f) * (4.0f / WIDTH);

                render_newton(params->newton.zoom, params->newton.offset_x,
                params->newton.offset_y, (int) params->newton.iterations, &params->newton);
                *update = false;
            }

            DrawTexture(params->newton.texture, 0, 0, WHITE);
            break;
        }
    }
}

void render_fractal_gui(Camera2D* cam, FractalParameters* params, const AppState* state, bool* update) {
    switch (*state) {
        case STATE_TREE:
            tree_gui(params, cam, update);
            break;
        case STATE_CARPET:
            carpet_gui(params, cam, update);
            break;
        case STATE_TRIANGLE:
            triangle_gui(params, cam, update);
            break;
        case STATE_MANDELBROT:
            mandelbrot_gui(params, cam, update);
            break;
        case STATE_JULIA:
            julia_gui(params, cam, update);
            break;
        case STATE_CIRCLE:
            circle_gui(params, cam, update);
            break;
        case STATE_FERN:
            fern_gui(params, cam, update);
            break;
        case STATE_NEWTON:
            newton_gui(params, cam, update);
            break;
        default:
            break;
    }
}

void save_image(const AppState state, const AppState random_type, const FractalParameters* params,
    const FractalParameters* random_params) {
    Image finalImage = {0};
    const AppState currentState = (state == STATE_RANDOM) ? random_type : state;
    const FractalParameters* currentParams = (state == STATE_RANDOM) ? random_params : params;

    switch (currentState) {
        case STATE_MANDELBROT:
            finalImage = LoadImageFromTexture(currentParams->mandelbrot.texture);
            break;
        case STATE_JULIA:
            finalImage = LoadImageFromTexture(currentParams->julia.texture);
            break;
        case STATE_CARPET:
            finalImage = LoadImageFromTexture(currentParams->carpet.texture);
            break;
        case STATE_FERN:
            finalImage = LoadImageFromTexture(currentParams->fern.texture);
            break;
        case STATE_NEWTON:
            finalImage = LoadImageFromTexture(currentParams->newton.texture);
            break;
        default:
            finalImage = LoadImageFromScreen();
            ImageCrop(&finalImage, (Rectangle){270.0f, 0.0f, (float)(WIDTH - 270), (float)HEIGHT});
            break;
    }

    if (finalImage.data != NULL) {
        char* savePath = Win32_ShowSaveDialog("fractal.png");
        if (savePath != NULL) {
            if (ExportImage(finalImage, savePath)) {
                TraceLog(LOG_INFO, "Изображение сохранено: %s", savePath);
            } else {
                TraceLog(LOG_ERROR, "Ошибка экспорта изображения");
            }
            free(savePath);
        }
        UnloadImage(finalImage);
    }
}