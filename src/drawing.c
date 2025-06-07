#include "drawing.h"

Color dim_color(const Color C, float Factor) {
    if (Factor < 0.0f || Factor > 1.0f) return C;
    Color R = C;
    R.a = (unsigned char)((float)C.a * Factor);
    return R;
}

void draw_background() {
    ClearBackground(COLOR_BACKGROUND);
}

void draw_circle_outline(Vector2 point, float radius, Color color) {
    DrawCircle     ((int)point.x, (int)point.y, radius, dim_color(color, 0.3f));
    DrawCircleLines((int)point.x, (int)point.y, radius,           color);
    DrawCircleLines((int)point.x, (int)point.y, radius * 0.99f,   color);
}

int draw(void){

    // Draw the background
    draw_background();

    // TEMP
    draw_circle_outline((Vector2){400, 400}, 40, (Color){255, 0, 255, 70});

    return 0;
}

