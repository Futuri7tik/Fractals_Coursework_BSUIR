#include <SFML/Graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 1920
#define HEIGHT 1080
#define MAX_ITER 100

typedef struct {
    double offset_x;
    double offset_y;
    double zoom;
} FractalParams;

// --- Функция Мандельброта ---
int mandelbrot(double cx, double cy) {
    double x = 0.0;
    double y = 0.0;
    int iteration = 0;

    while (x*x + y*y <= 4.0 && iteration < MAX_ITER) {
        double x_temp = x*x - y*y + cx;
        y = 2*x*y + cy;
        x = x_temp;
        iteration++;
    }

    return iteration;
}

// --- Цвет (сделал ярче) ---
sfColor get_color(int iteration) {
    if (iteration == MAX_ITER) {
        return sfBlack;
    }

    // Более яркие цвета
    unsigned char r = (iteration * 4) % 255;
    unsigned char g = (iteration * 8) % 255;
    unsigned char b = (iteration * 12) % 255;

    return sfColor_fromRGB(r, g, b);
}

// --- Отрисовка ---
void draw_fractal(sfImage* image, FractalParams* params) {
    int x, y;
    int colored_pixels = 0;  // Для отладки

    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            // Преобразование координат пикселя в комплексную плоскость
            double cx = (x - WIDTH / 2.0) / params->zoom + params->offset_x;
            double cy = (y - HEIGHT / 2.0) / params->zoom + params->offset_y;

            int iteration = mandelbrot(cx, cy);
            sfColor color = get_color(iteration);

            if (iteration != MAX_ITER) {
                colored_pixels++;
            }

            sfImage_setPixel(image, x, y, color);
        }
    }

    // Отладочный вывод
    printf("Drawn pixels: %d from %d (colored: %d)\n",
           WIDTH * HEIGHT, WIDTH * HEIGHT, colored_pixels);
}

int main(void) {
    sfVideoMode mode = {WIDTH, HEIGHT, 32};
    sfRenderWindow* window = sfRenderWindow_create(mode, "Mandelbrot", sfDefaultStyle, NULL);

    if (!window) {
        printf("Error: Could not create window!\n");
        return 1;
    }

    sfRenderWindow_setFramerateLimit(window, 60);

    // Изображение и текстура
    sfImage* image = sfImage_create(WIDTH, HEIGHT);
    sfTexture* texture = sfTexture_create(WIDTH, HEIGHT);
    sfSprite* sprite = sfSprite_create();

    sfSprite_setTexture(sprite, texture, sfTrue);

    // Параметры (использовал проверенные значения)
    FractalParams params = {
        .offset_x = -0.5,
        .offset_y = 0.0,
        .zoom = 300.0
    };

    printf("=== Mandelbrot Fractal ===\n");
    printf("Arrows - Movement\n");
    printf("Wheel - Zoom\n");
    printf("S - Screenshot\n");
    printf("ESC - Exit\n\n");

    // 🔥 ПЕРВАЯ ОТРИСОВКА (обязательно перед циклом!)
    printf("First draw...\n");
    draw_fractal(image, &params);
    printf("Updating textures...\n");
    sfTexture_updateFromImage(texture, image, 0, 0);
    printf("Done!\n\n");

    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        int needs_redraw = 0;

        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed) {
                sfRenderWindow_close(window);
            }

            if (event.type == sfEvtKeyPressed) {
                const int OFF = 20;
                switch (event.key.code) {
                    case sfKeyEscape:
                        sfRenderWindow_close(window);
                        break;
                    case sfKeyLeft:
                        params.offset_x -= OFF / params.zoom;
                        needs_redraw = 1;
                        break;
                    case sfKeyRight:
                        params.offset_x += OFF / params.zoom;
                        needs_redraw = 1;
                        break;
                    case sfKeyUp:
                        params.offset_y -= OFF / params.zoom;
                        needs_redraw = 1;
                        break;
                    case sfKeyDown:
                        params.offset_y += OFF / params.zoom;
                        needs_redraw = 1;
                        break;
                    case sfKeyS:
                        if (sfImage_saveToFile(image, "fractal.png")) {
                            printf("Saved: fractal.png\n");
                        }
                        break;
                }
            }

            if (event.type == sfEvtMouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sfMouseVerticalWheel) {
                    if (event.mouseWheelScroll.delta > 0) {
                        params.zoom *= 1.1;
                    } else {
                        params.zoom /= 1.1;
                    }
                    needs_redraw = 1;
                }
            }
        }

        if (needs_redraw) {
            draw_fractal(image, &params);
            sfTexture_updateFromImage(texture, image, 0, 0);
        }

        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawSprite(window, sprite, NULL);
        sfRenderWindow_display(window);
    }

    sfSprite_destroy(sprite);
    sfTexture_destroy(texture);
    sfImage_destroy(image);
    sfRenderWindow_destroy(window);

    return 0;
}