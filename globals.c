#include <SFML/Graphics.h>
#include "functions.h"

sfVertex vertices[MAX_VERTICES];
int vertex_count = 0;

void flush_fractal(sfRenderWindow* window, const sfPrimitiveType primitive) {
    sfRenderWindow_drawPrimitives(window, vertices, vertex_count, primitive, NULL);
    vertex_count = 0;
}