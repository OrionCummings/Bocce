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

void draw_court(void) {

    const float r = 25.0f;
    const float s = 7.0f;
    const float wall_roundness = 0.1f;
    const int wall_roundness_segments = 10;

    Vector2 a = (Vector2){ .x = 300, .y = 50 }; // top left
    Vector2 b = (Vector2){ .x = 900, .y = 750 }; // bot right
    Vector2 c = (Vector2){ .x = b.x, .y = a.y }; // top right
    Vector2 d = (Vector2){ .x = a.x, .y = b.y }; // bot left
    Vector2 size = (Vector2){ .x = b.x - a.x, .y = b.y - a.y };

    Rectangle court_rect = (Rectangle){ .x = a.x, .y = a.y, .width = size.x, .height = size.y };
    Rectangle court_rect_shadow_1 = (Rectangle){ .x = a.x, .y = a.y, .width = s, .height = d.y - a.y };
    Rectangle court_rect_shadow_2 = (Rectangle){ .x = a.x, .y = a.y, .width = c.x - a.x, .height = s };
    Rectangle court_rect_wall = (Rectangle){ .x = a.x - r, .y = a.y - r, .width = size.x + (2 * r), .height = size.y + (2 * r) };
    Rectangle court_rect_wall_shadow = (Rectangle){ .x = a.x - r + s, .y = a.y - r + s, .width = size.x + (2 * r), .height = size.y + (2 * r) };

    DrawRectangleRounded(court_rect_wall_shadow, wall_roundness, wall_roundness_segments, COLOR_COURT_WALL_DARK); // wall shadow (right)
    DrawRectangleRounded(court_rect_wall, wall_roundness, wall_roundness_segments, COLOR_COURT_WALL); // wall
    DrawRectangleRec(court_rect, COLOR_COURT_BASE); // court
    DrawRectangleRec(court_rect_shadow_1, COLOR_COURT_BASE_DARK); // court shadow (left)
    DrawRectangleRec(court_rect_shadow_2, COLOR_COURT_BASE_DARK); // court shadow (top)

}

void draw_balls(const Ball* balls, uint16_t num_balls) {
    for (size_t index = 0; index < num_balls; index++) {
        draw_ball(balls[index]);
    }
}

void draw_ball(const Ball ball) {
    draw_circle_outline(ball.position, ball.size, ball.color, 1.0f);
}

void draw_circle_outline(Vector2 point, float radius, Color color, float dim_factor) {
    DrawCircle((int)point.x, (int)point.y, radius, dim_color(color, dim_factor));
    DrawCircleLines((int)point.x, (int)point.y, radius, color);
    DrawCircleLines((int)point.x, (int)point.y, radius * 0.99f, color);
}

ErrorCode draw(const ApplicationSettings* settings, const GameState* state) {

    if (settings == NULL) { B_ERROR("Passed null parameter 'settings'"); return EC_PASSED_NULL; }
    if (state == NULL) { B_ERROR("Passed null parameter 'state'"); return EC_PASSED_NULL; }

    // Draw the background and the bocce court
    draw_background();
    draw_court();
    draw_balls(state->balls, state->num_balls);

    // Debug information
    draw_debug_information(settings);
    draw_circle_outline(GetMousePosition(), 20, (Color){ 255, 0, 255, 70 }, 0.3f);

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
    DrawText(TextFormat("Client: %s", (is_client(*settings)) ? "Yes" : "No"), 10, 10 + (20 * i++), 20, COLOR_TEXT);
    DrawText(TextFormat("Server: %s", (is_server(*settings)) ? "Yes" : "No"), 10, 10 + (20 * i++), 20, COLOR_TEXT);
    // DrawText(TextFormat("Connected to: %s", get_server_address(*)), 10, 10 + (20 * i++), 20, COLOR_TEXT);
}