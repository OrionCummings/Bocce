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

ErrorCode draw(const ApplicationSettings* settings, const GameState* state, const Chat* chat) {

    if (settings == NULL) { B_ERROR("Passed null parameter 'settings'"); return EC_PASSED_NULL; }
    if (state == NULL) { B_ERROR("Passed null parameter 'state'"); return EC_PASSED_NULL; }

    // Draw the background and the bocce court
    draw_background();
    // draw_court();
    // draw_balls(state->balls, state->num_balls);
    Vector2 chat_pos = (Vector2){ 300, 50 };
    Vector2 chat_dim = (Vector2){ 800, 700 };
    draw_chat(chat_pos, chat_dim, chat);

    // Debug information
    draw_debug_information(settings);
    draw_circle_outline(GetMousePosition(), 20, (Color){ 255, 0, 255, 70 }, 0.3f);

    return 0;
}

void draw_chat(const Vector2 pos, const Vector2 dim, const Chat* chat) {

    Vector2 shadow_offset = { 7, 7 };
    Vector2 input_offset = { 4, 4 };
    int input_height = 34;
    int padding = 4;

    // Draw shadow
    DrawRectangle((int)(pos.x + shadow_offset.x), (int)(pos.y + shadow_offset.y), (int)dim.x, (int)dim.y, COLOR_VSC_2);

    // Draw background
    DrawRectangle((int)pos.x, (int)pos.y, (int)dim.x, (int)dim.y, COLOR_VSC_3);

    // Draw prebuffer box
    int pb_x = (int)(pos.x + input_offset.x);
    int pb_y = (int)(pos.y + dim.y - ((float)input_height + input_offset.y));
    int pb_w = (int)(dim.x - (2 * input_offset.x));
    int pb_h = input_height;
    DrawRectangle(pb_x, pb_y, pb_w, pb_h, COLOR_VSC_4);

    // Draw message history
    for (uint16_t index = 0; index < chat->history.num_messages; index++) {
        ChatMessage message = chat->history.messages[index];
        char buffer[128] = { 0 };
        format_chat_message(buffer, message);
        DrawText(buffer, 500, 500, 30, COLOR_VSC_5);
    }

    // Draw prebuffer message
    ChatMessage hint_message = (ChatMessage){.text = "type a message!", .text_index = 15, .userId = 0};
    ChatMessage message = chat->pre_buffer;
    char buffer[128] = { 0 };
    
    // Display the chat hint if there is no text in the prebuffer
    if (chat->pre_buffer_index == 0) {
        format_chat_message(buffer, hint_message);
        DrawText(buffer, pb_x + (padding / 2), pb_y + (padding / 2), 32, COLOR_VSC_3);
    } else {
        format_chat_message(buffer, message);
        DrawText(buffer, pb_x + (padding / 2), pb_y + (padding / 2), 32, COLOR_VSC_5);
    }

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




