#include "drawing.h"

// static Camera2D camera = { 0 };

ErrorCode draw(const ApplicationSettings* settings, const GameState* state, const Chat* chat, const Font* font, RenderTexture screen_game) {

    if (settings == NULL) { B_ERROR("Passed null parameter 'settings'"); return EC_PASSED_NULL; }
    if (state == NULL) { B_ERROR("Passed null parameter 'state'"); return EC_PASSED_NULL; }
    if (chat == NULL) { B_ERROR("Passed null parameter 'chat'"); return EC_PASSED_NULL; }
    if (font == NULL) { B_ERROR("Passed null parameter 'font'"); return EC_PASSED_NULL; }

    Vector2 chat_pos = (Vector2){ 300, 50 };
    Vector2 chat_dim = (Vector2){ 800, 700 };

    // Draw the background and the bocce court
    draw_background();
    draw_game(settings->window_settings, screen_game);
    draw_game_info(settings->window_settings);
    draw_chat(settings->window_settings, chat_pos, chat_dim, chat, font);

    // Debug information
    draw_debug_information(settings);
    draw_circle_outline(GetMousePosition(), 20, (Color){ 255, 0, 255, 70 }, 0.3f);

    return 0;
}

void draw_game(const WindowSettings ws, RenderTexture screen) {

    float ui_width = ((float)ws.window_width * HORIZONTAL_RATIO) - (padding * 2);
    float ui_height = (float)(ws.window_height) - (padding * 2);
    Vector2 ui_anchor = (Vector2){ .x = origin.x + padding, .y = origin.y + padding };

    Vector2 mouse_position = (Vector2){GetMousePosition().x, ui_height - GetMousePosition().y + padding};

    Rectangle ui_rect = (Rectangle){
        .x = ui_anchor.x,
        .y = ui_anchor.y,
        .width = ui_width,
        .height = ui_height
    };

    Vector2 center = {
        .x = (ui_rect.x + ui_rect.width) / 2.0f,
        .y = (ui_rect.y + ui_rect.height) / 2.0f
    };

    BeginTextureMode(screen);
    ClearBackground(COLOR_BACKGROUND);
    
    draw_ui_base(ui_rect);
    draw_game_court(ui_rect);
    // draw_game_debug();

    // Draw court-related objects in the subscreen [0, 0] x [court_width, court_height]
    draw_circle_outline((Vector2){0.0f, 0.0f}, 100.0f, RED, 0.5f);
    draw_circle_outline((Vector2){ui_width, 0.0f}, 100.0f, GREEN, 0.5f);
    draw_circle_outline((Vector2){0.0f, ui_height}, 100.0f, BLUE, 0.5f);
    draw_circle_outline((Vector2){ui_width, ui_height}, 100.0f, PURPLE, 0.5f);
    draw_circle_outline((Vector2){ui_width / 2, ui_height / 2}, 100.0f, WHITE, 0.5f);
    draw_circle_outline(mouse_position, 100.0f, ORANGE, 0.5f);
    
    EndTextureMode();
    DrawTextureRec(screen.texture, ui_rect, ui_anchor, WHITE);
}

void draw_game_court(const Rectangle ui_rect) {
    const float court_padding = 100.0f;
    const float r = 5.0f;
    const float s = 7.0f;
    const float wall_roundness = 0.01f;
    const int wall_roundness_segments = 10;

    Vector2 a = (Vector2){ .x = ui_rect.x + court_padding, .y = ui_rect.y + court_padding }; // top left
    Vector2 b = (Vector2){ .x = ui_rect.x + ui_rect.width - court_padding, .y = ui_rect.y + ui_rect.height - court_padding }; // bot right
    Vector2 c = (Vector2){ .x = b.x, .y = a.y }; // top right
    Vector2 d = (Vector2){ .x = a.x, .y = b.y }; // bot left
    Vector2 size = (Vector2){ .x = b.x - a.x, .y = b.y - a.y };

    Rectangle court_rect = (Rectangle){ .x = a.x, .y = a.y, .width = size.x, .height = size.y };
    Rectangle court_rect_shadow_1 = (Rectangle){ .x = a.x, .y = a.y, .width = s, .height = d.y - a.y };
    Rectangle court_rect_shadow_2 = (Rectangle){ .x = a.x, .y = a.y, .width = c.x - a.x, .height = s };
    Rectangle court_rect_wall = (Rectangle){ .x = a.x - r, .y = a.y - r, .width = size.x + (2 * r), .height = size.y + (2 * r) };

    DrawRectangleRec(court_rect_shadow_1, COLOR_COURT_BASE_DARK); // court shadow (left)
    DrawRectangleRec(court_rect_shadow_2, COLOR_COURT_BASE_DARK); // court shadow (top)
    DrawRectangleRounded(court_rect_wall, wall_roundness, wall_roundness_segments, COLOR_COURT_WALL); // wall
    DrawRectangleRec(court_rect, COLOR_COURT_BASE); // court

}

void draw_chat(const WindowSettings ws, const Vector2 pos, const Vector2 dim, const Chat* chat, const Font* font) {

    if (chat == NULL) { B_ERROR("Passed null parameter 'chat'"); return; }
    if (font == NULL) { B_ERROR("Passed null parameter 'font'"); return; }

    const float chat_width = ((float)ws.window_width * (1.0f - HORIZONTAL_RATIO)) - (padding * 2);
    const float chat_height = (float)((float)ws.window_height * (1.0f - VERTICAL_RATIO)) - (padding * 2);

    Rectangle rect_chat_rect = (Rectangle){
        .x = origin.x + ((float)ws.window_width * HORIZONTAL_RATIO),
        .y = origin.y + ((float)ws.window_height * (1.0f - VERTICAL_RATIO)) + (padding / 2.0f),
        .width = chat_width + padding,
        .height = chat_height + (padding / 2.0f)
    };

    DrawRectangleRec(rect_shrink(rect_chat_rect, 10.0f), COLOR_VSC_3);
    // GuiGroupBox(rect_chat_rect, "Chat");
    draw_ui_base(rect_chat_rect);

    // Vector2 shadow_offset = { 7, 7 };
    // Vector2 input_offset = { 4, 4 };
    // const int input_height = 34;
    // const int padding = 4;
    // const int font_size = 30;

    // // Draw shadow
    // DrawRectangle((int)(pos.x + shadow_offset.x), (int)(pos.y + shadow_offset.y), (int)dim.x, (int)dim.y, COLOR_VSC_2);

    // // Draw background
    // DrawRectangle((int)pos.x, (int)pos.y, (int)dim.x, (int)dim.y, COLOR_VSC_3);

    // // Draw active text box
    // int pb_x = (int)(pos.x + input_offset.x);
    // int pb_y = (int)(pos.y + dim.y - ((float)input_height + input_offset.y));
    // int pb_w = (int)(dim.x - (2 * input_offset.x));
    // int pb_h = input_height;
    // DrawRectangle(pb_x, pb_y, pb_w, pb_h, COLOR_VSC_4);

    // // Draw message history
    // Vector2 text_pos = (Vector2){0};
    // for (uint16_t index = chat->history.num_messages; index > 0; index--) {
    //     ChatMessage message = get_message(chat, index - 1);
    //     char buffer[128] = { 0 };
    //     format_chat_message(buffer, message);
    //     text_pos = (Vector2){.x = (float)(pb_x + (padding / 2)), .y = (float)(pb_y - (font_size * (chat->history.num_messages - index + 1)))};
    //     DrawTextEx(*font, buffer, text_pos, (float)font->baseSize, (float)padding, COLOR_VSC_5);
    // }

    // // Draw active text message
    // ChatMessage hint_message = (ChatMessage){ .text = "type a message!", .text_size = 15, .userId = 0 };
    // ChatMessage message = chat->active_message;
    // char buffer[128] = { 0 };

    // // Display the chat hint if there is no active text
    // const Vector2 active_text_pos = (Vector2){ (float)(pb_x + (padding / 2)), (float)(pb_y + (padding / 2)) };
    // if (chat->active_message.text_size == 0) {
    //     format_chat_message(buffer, hint_message);
    //     DrawTextEx(*font, buffer, active_text_pos, (float)font->baseSize, (float)padding, COLOR_VSC_3);
    // } else {
    //     format_chat_message(buffer, message);
    //     DrawTextEx(*font, buffer, active_text_pos, (float)font->baseSize, (float)padding, COLOR_VSC_5);
    // }

}

void draw_game_info(const WindowSettings ws) {

    float bocce_game_info_width = ((float)ws.window_width * (1.0f - HORIZONTAL_RATIO)) - (padding * 2);
    float bocce_game_info_height = ((float)ws.window_height * VERTICAL_RATIO) - (padding * 2);

    Rectangle rect_bocce_game_info = (Rectangle){
        .x = origin.x + ((float)ws.window_width * HORIZONTAL_RATIO),
        .y = origin.y + padding,
        .width = bocce_game_info_width + padding,
        .height = bocce_game_info_height + (padding / 2.0f)
    };

    DrawRectangleRec(rect_shrink(rect_bocce_game_info, 10.0f), COLOR_VSC_3);
    // GuiGroupBox(rect_bocce_game_info, "Game Info");
    draw_ui_base(rect_bocce_game_info);
}

void draw_debug_information(const ApplicationSettings* settings) {

    int i = 0;
    const int offset_x = 20;
    const int offset_y = 20;

    Vector2 mp = GetMousePosition();

    Color color = COLOR_TEXT;
    int fps = GetFPS();
    if ((fps < 30) && (fps >= 15)) color = ORANGE;  // Warning FPS
    else if (fps < 15) color = RED;             // Low FPS
    DrawText(TextFormat("%2i FPS", fps), offset_x, offset_y + (20 * i++), 20, color);

    DrawText(TextFormat("Screen Size: [%i, %i]", GetScreenWidth(), GetScreenHeight()), offset_x, offset_y + (20 * i++), 20, COLOR_TEXT);
    DrawText(TextFormat("Mouse: [%i, %i]", (int)mp.x, (int)mp.y), offset_x, offset_y + (20 * i++), 20, COLOR_TEXT);
    DrawText(TextFormat("Client: %s", (is_client(*settings)) ? "Yes" : "No"), offset_x, offset_y + (20 * i++), 20, COLOR_TEXT);
    DrawText(TextFormat("Server: %s", (is_server(*settings)) ? "Yes" : "No"), offset_x, offset_y + (20 * i++), 20, COLOR_TEXT);
    // DrawText(TextFormat("Connected to: %s", get_server_address(*)), 10, 10 + (20 * i++), 20, COLOR_TEXT);
}

// Misc

Color dim_color(const Color C, float Factor) {
    if (Factor < 0.0f || Factor > 1.0f) return C;
    Color R = C;
    R.a = (unsigned char)((float)C.a * Factor);
    return R;
}

void draw_background() {
    ClearBackground(COLOR_BACKGROUND);
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

void draw_ui_base(Rectangle r) {

    // DEBUG: Draw dots on the corners!


    DrawRectangleRec(r, COLOR_VSC_2);
    DrawRectangleRec(rect_shrink(r, 5.0f), COLOR_VSC_3);
}

