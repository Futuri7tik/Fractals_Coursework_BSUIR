#include <math.h>
#include <raygui.h>
#include <stddef.h>
#include <stdlib.h>

#include "functions.h"

void init_fractals_parameters(FractalParameters* params) {
    init_tree_parameters(&params->tree);
    init_carpet_parameters(&params->carpet);
    init_triangle_parameters(&params->triangle);
    init_mandelbrot_parameters(&params->mandelbrot);
    init_julia_parameters(&params->julia);
}

void init_tree_parameters(TreeParameters* params) {
    params->depth = 14;
    params->max_depth = 18;
    params->angle = 25.0f * DEG2RAD;
    params->angle_degrees = 25.0f;
    params->length_factor = 0.75f;
}

void init_carpet_parameters(CarpetParameters* params) {
    params->depth = 5;
    params->max_depth = 8;
    params->start_length = HEIGHT - 200;
    params->red = 102.f;
    params->green = 191.f;
    params->blue = 255.f;
    params->texture = (Texture2D) {0};
}

void init_triangle_parameters(TriangleParameters* params) {
    params->depth = 9;
    params->max_depth = 13;
    params->start_length = HEIGHT - 50;
    params->x_start = (WIDTH - params->start_length) / 2.0f;
    params->y_start = HEIGHT - (HEIGHT - params->start_length * sqrtf(3.0f) / 2.0f) / 2.0f;
    params->red = 255.0f;
    params->green = 255.0f;
    params->blue = 0.0f;
}

void init_mandelbrot_parameters(MandelbrotParameters* params) {
    params->iterations = 200;
    params->max_iterations = 800;
    params->zoom = 1.0f;
    params->offset_x = 0.0f;
    params->offset_y = 0.0f;
    params->red = 9;
    params->green = 15;
    params->blue = 8.5f;
    params->texture = (Texture2D) {0};
}

void init_julia_parameters(JuliaParameters* params) {
    params->iterations = 200;
    params->max_iterations = 800;
    params->zoom = 1.0f;
    params->offset_x = 0.0f;
    params->offset_y = 0.0f;
    params->re_c = -0.8f;
    params->im_c = 0.156f;
    params->red = 9;
    params->green = 15;
    params->blue = 8.5f;
    params->texture = (Texture2D) {0};
}

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
             "- Julia Set\n\n"
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

void gallery_gui(AppState* state, FractalParameters* params, Camera2D* cam, ImageNode** head_img, bool* update) {
    GuiPanel((Rectangle){0, 10, WIDTH, HEIGHT - 10}, "Fractal Gallery");
    cam->zoom = 1.0f;
    cam->target = (Vector2){WIDTH/2.0f, HEIGHT/2.0f};

    char* fractal_names[] = {"Mandelbrot Set", "Pythagorean Tree","Sierpinski Carpet", "Sierpinski Triangle", "Julia Set"};
    char* image_names[] = {"mandelbrot.png", "tree.png", "carpet.png", "triangle.png", "julia.png"};
    Rectangle fields[] = {{0, 0,0,0}, {0, 0,0,0},{0, 0,0,0},
        {0, 0,0,0}, {0, 0,0,0}};
    size_t size = sizeof(fractal_names) / sizeof(fractal_names[0]);

    load_gallery(head_img, fractal_names, image_names, size, fields);
    draw_pics(*head_img);

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x00000000);
    if (GuiButton(fields[0], "")) {
        *state = STATE_MANDELBROT;
        *update = true;
        params->mandelbrot.zoom = cam->zoom;
        cam->target = (Vector2){WIDTH / 2.0f - 300.0f, HEIGHT / 2.0f};
        params->mandelbrot.offset_x = (cam->target.x - WIDTH / 2.0f) / WIDTH;
        params->mandelbrot.offset_y = (cam->target.y - HEIGHT / 2.0f) / HEIGHT;
    }

    if (GuiButton(fields[1], ""))
        *state = STATE_TREE;

    if (GuiButton(fields[2], "")) {
        *state = STATE_CARPET;
        *update = true;
    }

    if (GuiButton(fields[3], "")) {
        *state = STATE_TRIANGLE;
    }

    if (GuiButton(fields[4], "")) {
        *state = STATE_JULIA;
        *update = true;
        params->julia.zoom = cam->zoom;
        cam->target = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
        params->julia.offset_x = (cam->target.x - WIDTH/2.0f) / WIDTH;
        params->julia.offset_y = (cam->target.y - HEIGHT/2.0f) / HEIGHT;
    }
}

void tree_gui(FractalParameters* params, Camera2D* cam) {
    GuiLabel((Rectangle){20, 50, 200, 20}, TextFormat("Depth: %d", (int) params->tree.depth));

    GuiSlider((Rectangle){20, 80, 200, 20}, NULL, NULL,
        &params->tree.depth, 1,(float)params->tree.max_depth);
    params->tree.depth = (float) (int)params->tree.depth;

    GuiLabel((Rectangle){20, 120, 200, 20}, TextFormat("Angle: %.1f°", params->tree.angle_degrees));
    GuiSlider((Rectangle){20, 150, 200, 20}, NULL, NULL,
        &params->tree.angle_degrees, 0, 90);
    params->tree.angle = params->tree.angle_degrees * DEG2RAD;

    GuiLabel((Rectangle){20, 190, 200, 20}, TextFormat("Length Factor: %.2f", params->tree.length_factor));
    GuiSlider((Rectangle){20, 220, 200, 20}, NULL, NULL,
        &params->tree.length_factor, 0, 0.9f);

    if (GuiButton((Rectangle){20, 370, 110, 30}, "Reset")) {
        init_tree_parameters(&params->tree);
        cam->zoom = 1.0f;
        cam->target = (Vector2){WIDTH/2.0f, HEIGHT/2.0f};
    }
}

void carpet_gui(FractalParameters* params, Camera2D* cam, bool* update) {
    GuiLabel((Rectangle){20, 50, 200, 20}, TextFormat("Depth: %d", (int) params->carpet.depth));
    if (GuiSlider((Rectangle){20, 80, 200, 20}, NULL, NULL,
        &params->carpet.depth, 0, (float) params->carpet.max_depth))
        *update = true;

    params->carpet.depth = (float) (int) params->carpet.depth;

    GuiLabel((Rectangle){20, 110, 200, 20}, TextFormat("Palette:"));
    GuiLabel((Rectangle){20, 130, 200, 20}, TextFormat("Red factor: "));
    if (GuiSlider((Rectangle){20, 150, 200, 20}, NULL, NULL,
                  &params->carpet.red, 0, 255)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 170, 200, 20}, TextFormat("Green factor:"));
    if (GuiSlider((Rectangle){20, 190, 200, 20}, NULL, NULL,
                  &params->carpet.green, 0, 255)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 210, 200, 20}, TextFormat("Blue factor:"));
    if (GuiSlider((Rectangle){20, 230, 200, 20}, NULL, NULL,
        &params->carpet.blue, 0, 255)) {
        *update = true;
    }

    if (GuiButton((Rectangle){20, 370, 110, 30}, "Reset")) {
        init_carpet_parameters(&params->carpet);
        cam->zoom = 1.0f;
        cam->target = (Vector2){WIDTH/2.0f, HEIGHT/2.0f};
        *update = true;
    }
}

void triangle_gui(FractalParameters* params, Camera2D* cam) {
    GuiLabel((Rectangle){20, 50, 200, 20}, TextFormat("Depth: %d", (int) params->triangle.depth));
    GuiSlider((Rectangle){20, 80, 200, 20}, NULL, NULL,
        &params->triangle.depth, 0, (float) params->triangle.max_depth);
    params->triangle.depth = (float) (int) params->triangle.depth;

    GuiLabel((Rectangle){20, 110, 200, 20}, TextFormat("Palette:"));

    GuiLabel((Rectangle){20, 130, 200, 20}, TextFormat("Red component: "));
    GuiSlider((Rectangle){20, 150, 200, 20}, NULL, NULL,
        &params->triangle.red, 0, 255);

    GuiLabel((Rectangle){20, 170, 200, 20}, TextFormat("Green component: :"));
    GuiSlider((Rectangle){20, 190, 200, 20}, NULL, NULL,
        &params->triangle.green, 0, 255);

    GuiLabel((Rectangle){20, 210, 200, 20}, TextFormat("Blue component: "));
    GuiSlider((Rectangle){20, 230, 200, 20}, NULL, NULL,
                  &params->triangle.blue, 0, 255);

    if (GuiButton((Rectangle){20, 370, 110, 30}, "Reset")) {
        init_triangle_parameters(&params->triangle);
        cam->zoom = 1.0f;
        cam->target = (Vector2){WIDTH/2.0f, HEIGHT/2.0f};
    }
}

void mandelbrot_gui(FractalParameters* params, Camera2D* cam, bool* update) {
    GuiLabel((Rectangle){20, 50, 200, 20}, TextFormat("Iterations: %d", (int) params->mandelbrot.iterations));
    if (GuiSlider((Rectangle){20, 80, 200, 20}, NULL, NULL,
                  &params->mandelbrot.iterations, 0, 500)) {
        params->mandelbrot.iterations = (float) (int) params->mandelbrot.iterations;
        *update = true;
    }

    GuiLabel((Rectangle){20, 110, 200, 20}, TextFormat("Palette:"));
    GuiLabel((Rectangle){20, 130, 200, 20}, TextFormat("Red factor: "));
    if (GuiSlider((Rectangle){20, 150, 200, 20}, NULL, NULL,
                  &params->mandelbrot.red, 0, 20)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 170, 200, 20}, TextFormat("Green factor:"));
    if (GuiSlider((Rectangle){20, 190, 200, 20}, NULL, NULL,
                  &params->mandelbrot.green, 0, 20)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 210, 200, 20}, TextFormat("Blue factor:"));
    if (GuiSlider((Rectangle){20, 230, 200, 20}, NULL, NULL,
                  &params->mandelbrot.blue, 0, 20)) {
        *update = true;
    }

    if (GuiButton((Rectangle){20, 370, 110, 30}, "Reset")) {
        init_mandelbrot_parameters(&params->mandelbrot);
        cam->target = (Vector2){WIDTH / 2.0f - 300.0f, HEIGHT / 2.0f};
        cam->offset = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
        cam->zoom = 1.0f;
        *update = true;
    }
}

void julia_gui(FractalParameters* params, Camera2D* cam, bool* update) {
    GuiLabel((Rectangle){20, 50, 200, 20}, TextFormat("Iterations: %d", (int) params->julia.iterations));
    if (GuiSlider((Rectangle){20, 80, 200, 20}, NULL, NULL,
                  &params->julia.iterations, 0, 500)) {
        params->julia.iterations = (float) (int) params->julia.iterations;
        *update = true;
    }

    GuiLabel((Rectangle){20, 110, 200, 20}, TextFormat("Palette:"));
    GuiLabel((Rectangle){20, 130, 200, 20}, TextFormat("Red factor: "));
    if (GuiSlider((Rectangle){20, 150, 200, 20}, NULL, NULL,
                  &params->julia.red, 0, 20)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 170, 200, 20}, TextFormat("Green factor:"));
    if (GuiSlider((Rectangle){20, 190, 200, 20}, NULL, NULL,
                  &params->julia.green, 0, 20)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 210, 200, 20}, TextFormat("Blue factor:"));
    if (GuiSlider((Rectangle){20, 230, 200, 20}, NULL, NULL,
                  &params->julia.blue, 0, 20)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 260, 200, 20}, TextFormat("Constant c: "));
    GuiLabel((Rectangle){20, 280, 200, 20}, TextFormat("Real part:"));
    if (GuiSlider((Rectangle){20, 300, 200, 20}, NULL, NULL,
                  &params->julia.re_c, -1, 1)) {
        *update = true;
    }

    GuiLabel((Rectangle){20, 320, 200, 20}, TextFormat("Complex part:"));
    if (GuiSlider((Rectangle){20, 340, 200, 20}, NULL, NULL,
                  &params->julia.im_c, -1, 1)) {
        *update = true;
    }

    if (GuiButton((Rectangle){20, 370, 110, 30}, "Reset")) {
        init_julia_parameters(&params->julia);
        cam->target = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
        cam->offset = (Vector2){WIDTH / 2.0f, HEIGHT / 2.0f};
        cam->zoom = 1.0f;
        *update = true;
    }
}

ImageNode* create_image_node(char* fract_name, char* img_name, Rectangle field, Texture2D texture) {
    ImageNode* node = malloc(sizeof(ImageNode));
    node->img_name = img_name;
    node->fract_name = fract_name;
    node->field = field;
    node->texture = texture;
    node->texture = texture;
    node->next = NULL;
    return node;
}

void load_gallery(ImageNode** head, char* fract_names[], char* img_names[], const size_t size, Rectangle* img_fields)
{
    *head = create_image_node("", "", (Rectangle) {0,0,0,0}, (Texture2D) {0});
    ImageNode* last = *head;

    for (size_t i = 0; i < size; ++i) {
        const int img_height = 230, img_width = 280;
        const float y_start = 100, x_start = 150;

        Image temp_img = LoadImage(img_names[i]);
        ImageResize(&temp_img, img_width, img_height);
        const Rectangle field = (Rectangle) {x_start + (float) (i % 5) * 350, y_start + (int) (i / 5) * 350, img_width, img_height};
        img_fields[i] = field;
        ImageNode* temp = create_image_node(fract_names[i], img_names[i],
            field, LoadTextureFromImage(temp_img));
        UnloadImage(temp_img);

        last->next = temp;
        last = temp;
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