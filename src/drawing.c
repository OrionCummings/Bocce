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
    DrawCircle((int)point.x, (int)point.y, radius, dim_color(color, 0.3f));
    DrawCircleLines((int)point.x, (int)point.y, radius, color);
    DrawCircleLines((int)point.x, (int)point.y, radius * 0.99f, color);
}

ErrorCode draw(const ApplicationSettings* settings){

    if (settings == NULL) {
        B_ERROR("Passed null parameter 'settings'");
        return EC_PASSED_NULL;
    }

    // Draw the background
    draw_background();

    // Debug information
    draw_debug_information(settings);
    draw_circle_outline(GetMousePosition(), 40, (Color){ 255, 0, 255, 70 });

    return 0;
}

void draw_debug_information(const ApplicationSettings* settings) {
    
    int i = 0;
    
    Color color = COLOR_TEXT;
    int fps = GetFPS();
    if ((fps < 30) && (fps >= 15)) color = ORANGE;  // Warning FPS
    else if (fps < 15) color = RED;             // Low FPS
    DrawText(TextFormat("%2i FPS", fps), 10, 10 + (20 * i++), 20, color);

    DrawText(TextFormat("Screen Size: [%i, %i]", GetScreenWidth(), GetScreenHeight()), 10, 10 + (20 * i++), 20, COLOR_TEXT);
    DrawText(TextFormat("Client: %s", (is_client(*settings))? "Yes" : "No"), 10, 10 + (20 * i++), 20, COLOR_TEXT);
    DrawText(TextFormat("Server: %s", (is_server(*settings))? "Yes" : "No"), 10, 10 + (20 * i++), 20, COLOR_TEXT);
    // DrawText(TextFormat("Connected to: %s", get_server_address(*)), 10, 10 + (20 * i++), 20, COLOR_TEXT);
}