#include <SFML/Graphics.h>
#include <stdio.h>
#include "functions.h"

void add_square(const double x, const double y, const double length, const sfColor color) {
    if (vertex_count + 4 > MAX_VERTICES) return;

    vertices[vertex_count++] = (sfVertex){{(float)x, (float)y}, color};
    vertices[vertex_count++] = (sfVertex){{(float) (x + length), (float)y}, color};
    vertices[vertex_count++] = (sfVertex){{(float) (x + length), (float) (y + length)}, color};
    vertices[vertex_count++] = (sfVertex){{(float)x, (float) (y + length)}, color};
}

sfColor get_color_carpet(const int depth, CarpetParameters* params) {
    const int color_1[3] = {0, 0, 0};
    const int color_2[3] = {0, 0, 153};
    const double t = 1 - (float) depth / (float) params->depth;

    const unsigned char r = color_1[0] * (1 - t) + color_2[0] * t;
    const unsigned char g = color_1[1] * (1 - t) + color_2[1] * t;
    const unsigned char b = color_1[2] * (1 - t) + color_2[2] * t;

    return sfColor_fromRGB(r, g, b);
}

void draw_carpet (sfRenderWindow* window, double x_left, double y_left,
    const double length, int depth, CarpetParameters* carpet) {
    depth -= 1;

    if (depth < 0) {
        return;
    }

    const double new_length = length / 3.0;
    const double x_draw = x_left + new_length;
    const double y_draw = y_left + new_length;

    const sfColor color = get_color_carpet(depth, carpet);
    add_square(x_draw, y_draw, new_length, color);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i != 1 || j != 1)
                draw_carpet(window, x_left + i * new_length, y_left + j * new_length, new_length, depth, carpet);
        }
    }
}

int render_carpet(void) {
    vertex_count = 0;

    const int WIDTH = 1920;
    const int HEIGHT = 1080;
    const int START_DEPTH = 7;
    const int MAX_DEPTH = 8;

    const sfVideoMode mode = {WIDTH, HEIGHT, 32};
    sfRenderWindow* window = sfRenderWindow_create(mode, "Sierpinski Carpet", sfDefaultStyle, NULL);
    CarpetParameters carpet = {START_DEPTH,MAX_DEPTH,HEIGHT - 200};

    if (!window)
        return -1;

    sfView* view = sfView_create();
    sfView_setSize(view, (sfVector2f){WIDTH, HEIGHT});
    sfView_setCenter(view, (sfVector2f){WIDTH / 2.0, HEIGHT / 2.0});

    sfEvent event;

    const double x_start = (WIDTH - carpet.start_length) / 2.0;
    const double y_start = (HEIGHT - carpet.start_length) / 2.0;
    float OFFSET = 20.0f;
    float ZOOM_FACTOR = 1;

    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            switch (event.type) {
                case sfEvtClosed:
                    sfView_destroy(view);
                    sfRenderWindow_close(window);
                    sfRenderWindow_destroy(window);
                    return 0;
                case sfEvtKeyPressed:
                    switch (event.key.code) {
                        case sfKeyEscape:
                            sfView_destroy(view);
                            sfRenderWindow_close(window);
                            sfRenderWindow_destroy(window);
                            return 0;
                        case sfKeyW:
                            carpet.depth += 1;
                            if (carpet.depth > carpet.max_depth) {
                                carpet.depth = carpet.max_depth;
                            }
                            break;
                        case sfKeyS:
                            carpet.depth -= 1;
                            if (carpet.depth < 0)
                                carpet.depth = 0;
                            break;
                        case sfKeyUp:
                            sfView_move(view, (sfVector2f) {0, -OFFSET});
                            sfRenderWindow_setView(window, view);
                            break;
                        case sfKeyDown:
                            sfView_move(view, (sfVector2f) {0, OFFSET});
                            sfRenderWindow_setView(window, view);
                            break;
                        case sfKeyLeft:
                            sfView_move(view, (sfVector2f) {(-OFFSET), 0});
                            sfRenderWindow_setView(window, view);
                            break;
                        case sfKeyRight:
                            sfView_move(view, (sfVector2f) {OFFSET, 0});
                            sfRenderWindow_setView(window, view);
                            break;
                    }
                    break;
                case sfEvtMouseWheelScrolled:
                    if (event.mouseWheelScroll.delta > 0) {
                        sfView_zoom(view, ZOOM_FACTOR - 0.05f);
                        OFFSET -= 0.05f * OFFSET;
                    }
                    else {
                        sfView_zoom(view, ZOOM_FACTOR + 0.05f);
                        OFFSET += 0.05f * OFFSET;
                    }
                    sfRenderWindow_setView(window, view);
                    break;
                }
            }
        sfRenderWindow_clear(window, sfBlack);

        vertex_count = 0;
        add_square(x_start,
            y_start, carpet.start_length, sfBlue);
        draw_carpet(window, x_start,y_start,carpet.start_length,carpet.depth, &carpet);

        flush_fractal(window, sfQuads);
        sfRenderWindow_display(window);
    }
    return 0;
}