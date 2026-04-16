#include <raylib.h>
#include <stdio.h>

void draw_base_circle(int x, int y, float r) {
    DrawCircle(x, y, r, WHITE);
    DrawCircle(x, y, r / 3, WHITE);

}

