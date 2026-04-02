#include <math.h>
#include <SFML/Graphics.h>
#include <stdio.h>
#include "functions.h"

void add_base(const double x, const double y, const double length, const sfColor color) {
    vertices[vertex_count++] = (sfVertex){{(float)x, (float)y}, color};
    vertices[vertex_count++] = (sfVertex){{(float) (x + length), (float)y}, color};
    vertices[vertex_count++] = (sfVertex){{(float)(x + length / 2.0),
        (float) (y - length * sqrt(3)/ 2.0)}, color};
}

void add_triangle(double x, double y, double length, sfColor color) {
    if (vertex_count + 3 > MAX_VERTICES) return;

    const double HEIGHT = length * sqrt(3) / 2.0;

    vertices[vertex_count++] = (sfVertex){{(float)x, (float)y}, color};
    vertices[vertex_count++] = (sfVertex){{(float) (x - length / 2.0), (float) (y - HEIGHT)}, color};
    vertices[vertex_count++] = (sfVertex){{(float)(x + length / 2.0),
        (float) (y - HEIGHT)}, color};
}

sfColor get_color_triangle(const int depth, TriangleParameters* params) {
    const int color_1[3] = {0, 0, 0};
    const int color_2[3] = {0, 0, 153};
    const double t = 1 - (float) depth / (float) params->depth;

    const unsigned char r = color_1[0] * (1 - t) + color_2[0] * t;
    const unsigned char g = color_1[1] * (1 - t) + color_2[1] * t;
    const unsigned char b = color_1[2] * (1 - t) + color_2[2] * t;

    return sfColor_fromRGB(r, g, b);
}

void draw_sierpinski_triangle(sfRenderWindow* window, double x, double y,
    const double length, int depth, TriangleParameters* params) {
    depth -= 1;

    if (depth < 0) {
        return;
    }

    double height = length * sqrt(3) / 2.0;
    double new_length = length / 2.0;

    add_triangle(x + length / 2.0, y, length / 2.0, get_color_triangle(depth, params));

    // left triangle
    draw_sierpinski_triangle(window, x, y, new_length, depth, params);

    // right triangle
    draw_sierpinski_triangle(window, x + length / 2.0, y, new_length, depth, params);

    // upper triangle
    draw_sierpinski_triangle(window, x + length / 4.0, y - height / 2.0, new_length, depth, params);
}

int render_triangle(void) {
    vertex_count = 0;

    const int WIDTH = 1920;
    const int HEIGHT = 1080;
    const int START_DEPTH = 9;
    const int START_LENGTH = HEIGHT - 50;
    const int MAX_DEPTH = 13;

    const sfVideoMode mode = {WIDTH, HEIGHT, 32};
    sfRenderWindow* window = sfRenderWindow_create(mode, "Sierpinski Triangle", sfDefaultStyle, NULL);
    TriangleParameters triangle = {START_DEPTH,MAX_DEPTH, START_LENGTH};

    if (!window)
        return -1;

    sfView* view = sfView_create();
    sfView_setSize(view, (sfVector2f){WIDTH, HEIGHT});
    sfView_setCenter(view, (sfVector2f){WIDTH / 2.0, HEIGHT / 2.0});

    const double START_HEIGHT = START_LENGTH * sqrt(3) / 2.0;
    const double x_start = (WIDTH - START_LENGTH) / 2.0;
    const double y_start = HEIGHT - (HEIGHT - START_HEIGHT) / 2.0;
    float OFFSET = 20.0f;
    float ZOOM_FACTOR = 1;
    sfEvent event;

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
                            triangle.depth += 1;
                            if (triangle.depth > triangle.max_depth) {
                                triangle.depth = triangle.max_depth;
                            }
                            break;
                        case sfKeyS:
                            triangle.depth -= 1;
                            if (triangle.depth < 0)
                                triangle.depth = 0;
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
        add_base(x_start,
            y_start, triangle.start_length, sfYellow);
        draw_sierpinski_triangle(window, x_start,y_start,triangle.start_length,triangle.depth, &triangle);

        flush_fractal(window, sfTriangles);
        sfRenderWindow_display(window);
    }
    return 0;
}