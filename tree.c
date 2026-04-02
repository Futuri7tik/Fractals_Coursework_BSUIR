#include <SFML/Graphics.h>
#include <stdio.h>
#include <math.h>
#include "functions.h"

sfColor get_color_tree(const int depth, const TreeParameters* parameters) {
    const int color_brown[3] = {102, 51, 0};
    const int color_green[3] = {0, 255, 0};
    const double t = 1 - depth / (parameters->start_depth * 1.0);

    const unsigned char r = color_brown[0] * (1 - t) + color_green[0] * t;
    const unsigned char g = color_brown[1] * (1 - t) + color_green[1] * t;
    const unsigned char b = color_brown[2] * (1 - t) + color_green[2] * t;

    return sfColor_fromRGB(r, g, b);
}
        
void add_line(const double x_start, const double y_start, const double x_end, const double y_end, const sfColor color) {
    vertices[vertex_count++] = (sfVertex) {{(float) x_start, (float) y_start}, color};
    vertices[vertex_count++] = (sfVertex) {{(float) x_end, (float) y_end}, color};
}

void draw_tree(sfRenderWindow* window, const double x_start,
    const double y_start, const double length, const double angle, int depth,
    TreeParameters* parameters) {
    depth -= 1;

    if (depth < 0) {
        return;
    }

    const double x_end = x_start + cos(angle) * length;
    const double y_end = y_start + sin(angle) * length;

    const sfColor color = get_color_tree(depth, parameters);

    add_line(x_start, y_start, x_end, y_end, color);

    const double new_length = length * parameters->length_factor;
    const double angle_left = angle - parameters->angle;
    const double angle_right = angle + parameters->angle;

    // left branch
    draw_tree(window, x_end, y_end, new_length, angle_left, depth, parameters);

    // right branch
    draw_tree(window, x_end, y_end, new_length, angle_right, depth, parameters);
}

int render_tree(void) {
    vertex_count = 0;

    const double FACTOR = 0.01;
    const double MAX_FACTOR = 5;
    const double START_FACTOR = 0.75;
    const double START_ANGLE = 25 * M_PI / 180;
    const int START_DEPTH = 14;
    const int MAX_DEPTH = 19;
    const double DEGREE_MOVE = 0.6;
    const int WIDTH = 1920;
    const int HEIGHT = 1080;

    TreeParameters parameters = {
        .depth = START_DEPTH,
        .start_depth = START_DEPTH,
        .angle = START_ANGLE,
        .length_factor = START_FACTOR,
    };

    sfVideoMode mode = {WIDTH, HEIGHT, 32};
    sfRenderWindow* window = sfRenderWindow_create(mode, "Tree", sfDefaultStyle, NULL);

    if (!window)
        return -1;

    sfView* view = sfView_create();
    sfView_setSize(view, (sfVector2f){WIDTH, HEIGHT});
    sfView_setCenter(view, (sfVector2f){WIDTH / 2.0, HEIGHT / 2.0});

    double OFFSET = 20.0f;
    float ZOOM_FACTOR = 1;

    sfEvent event;


    while (sfRenderWindow_isOpen(window)) {
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfView_destroy(view);
                sfRenderWindow_close(window);
                sfRenderWindow_destroy(window);
                return 0;
            }
            if (event.type == sfEvtKeyPressed) {
                switch (event.key.code) {
                    case sfKeyEscape:
                        sfView_destroy(view);
                        sfRenderWindow_close(window);
                        sfRenderWindow_destroy(window);
                        return 0;
                    case sfKeyUp:
                        if (parameters.depth < MAX_DEPTH) {
                            parameters.depth++;
                            parameters.start_depth++;
                        }
                        break;
                    case sfKeyDown:
                        if (parameters.depth > 1) {
                            parameters.depth--;
                            parameters.start_depth--;
                        }
                        break;
                    case sfKeyLeft:
                        parameters.angle -= DEGREE_MOVE * M_PI / 180;
                        break;
                    case sfKeyRight:
                        parameters.angle += DEGREE_MOVE * M_PI / 180;
                        break;
                    case sfKeyW:
                        parameters.length_factor += FACTOR;
                        if (parameters.length_factor > MAX_FACTOR)
                            parameters.length_factor = MAX_FACTOR;
                        break;
                    case sfKeyS:
                        parameters.length_factor -= FACTOR;
                        if (parameters.length_factor < 0)
                            parameters.length_factor = 0;
                        break;
                    case sfKeyT:
                        sfView_move(view, (sfVector2f) {0, -OFFSET});
                        sfRenderWindow_setView(window, view);
                        break;
                    case sfKeyG:
                        sfView_move(view, (sfVector2f) {0, OFFSET});
                        sfRenderWindow_setView(window, view);
                        break;
                    case sfKeyF:
                        sfView_move(view, (sfVector2f) {-OFFSET, 0});
                        sfRenderWindow_setView(window, view);
                        break;
                    case sfKeyH:
                        sfView_move(view, (sfVector2f) {OFFSET, 0});
                        sfRenderWindow_setView(window, view);
                        break;
                    case sfKeyR:
                        parameters.length_factor = START_FACTOR;
                        parameters.angle = START_ANGLE;
                        sfView_move(view, (sfVector2f) {0, 0});
                        sfView_setSize(view, (sfVector2f){WIDTH, HEIGHT});
                        sfView_setCenter(view, (sfVector2f){WIDTH / 2.0, HEIGHT / 2.0});
                        sfRenderWindow_setView(window, view);
                }
            }
            else if (event.type == sfEvtMouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) {
                    sfView_zoom(view, ZOOM_FACTOR - 0.05f);
                    OFFSET -= 0.05 * OFFSET;
                }
                else {
                    sfView_zoom(view, ZOOM_FACTOR + 0.05f);
                    OFFSET += 0.05 * OFFSET;
                }
                sfRenderWindow_setView(window, view);
                break;

            }
        }

        sfRenderWindow_clear(window, sfBlack);
        vertex_count = 0;

        draw_tree(window,
                  WIDTH / 2.0, HEIGHT - 220,
                  200,
                  -M_PI / 2.0,
                  parameters.depth,
                  &parameters);

        flush_fractal(window, sfLines);
        sfRenderWindow_display(window);

    }

    return 0;
}