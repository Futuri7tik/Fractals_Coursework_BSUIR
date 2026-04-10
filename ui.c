#include <raygui.h>
#include <stddef.h>
#include <stdlib.h>

#include "functions.h"

void init_tree_parameters(TreeParameters* params) {
    params->depth = 14;
    params->max_depth = 18;
    params->angle = 25.0f * DEG2RAD;
    params->angle_degrees = 25.0f;
    params->length_factor = 0.75f;
}

void init_carpet_parameters(CarpetParameters* params) {
    params->depth = 7;
    params->max_depth = 8;
    params->start_length = HEIGHT - 200;
    params->texture = (Texture2D) {0};
}

void init_triangle_parameters(TriangleParameters* params) {
    params->depth = 9;
    params->max_depth = 13;
    params->start_length = HEIGHT - 50;
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

void menu_gui(int* state, int* show_msg_box, bool* should_close) {
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

    if (show_msg_box) {
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
            show_msg_box = false;
            GuiSetStyle(gallery_button, TEXT_SIZE, button_text);
        }

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
        Rectangle field = (Rectangle) {x_start + (float) (i % 5) * 350, y_start + (int) (i / 5) * 350, img_width, img_height};
        img_fields[i] = field;
        ImageNode* temp = create_image_node(fract_names[i], img_names[i],
            field, LoadTextureFromImage(temp_img));
        UnloadImage(temp_img);

        last->next = temp;
        last = temp;
    }
}

void draw_pics(ImageNode* head) {
    int font_size = 24;
    ImageNode* temp = head;

    while (temp->next != NULL) {
        temp = temp->next;
        char* text = temp->fract_name;
        int textWidth = MeasureText(text, font_size);
        DrawText(text, (temp->field.width - textWidth) / 2 + temp->field.x, (int) temp->field.y - 25, font_size, GRAY);
        DrawTexture(temp->texture, (int) temp->field.x, (int) temp->field.y, WHITE);
    }
}