#include "drawing.h"

// static Camera2D camera = { 0 };

ErrorCode draw(const ApplicationSettings* settings, const GameState* state, const Chat* chat, const Font* font, RenderTexture screen_game, RenderTexture screen_game_info, RenderTexture screen_chat) {

    if (settings == NULL) { B_ERROR("Passed null parameter 'settings'"); return EC_PASSED_NULL; }
    if (state == NULL) { B_ERROR("Passed null parameter 'state'"); return EC_PASSED_NULL; }
    if (chat == NULL) { B_ERROR("Passed null parameter 'chat'"); return EC_PASSED_NULL; }
    if (font == NULL) { B_ERROR("Passed null parameter 'font'"); return EC_PASSED_NULL; }

    // Draw the background and the bocce court
    draw_background();
    draw_game(settings, screen_game);
    draw_game_info(settings, screen_game_info);
    draw_chat(settings, chat, font, screen_chat);

    // Draw top-level UI elements

    ///*
    float thickness = 4.0f;
    Color color = COLOR_VSC_2;

    Vector2 line_v_top = {
        ((float)settings->window_settings.window_width * HORIZONTAL_RATIO) - (thickness / 2.0f),
        0,
    };

    Vector2 line_v_bot = {
        line_v_top.x,
        (float)settings->window_settings.window_height,
    };

    Vector2 line_h_left = {
        ((float)settings->window_settings.window_width * HORIZONTAL_RATIO),
        ((float)settings->window_settings.window_height * VERTICAL_RATIO)
    };

    Vector2 line_h_right = {
        line_h_left.x + ((float)settings->window_settings.window_width * (1.0f - HORIZONTAL_RATIO)),
        line_h_left.y
    };

    DrawLineEx(line_v_top, line_v_bot, thickness, color);
    DrawLineEx(line_h_left, line_h_right, thickness, color);
    //*/

    // Debug information
    draw_debug_information(settings);
    draw_circle_outline(GetMousePosition(), 20, (Color){ 255, 0, 255, 70 }, 0.3f);
    draw_circle_outline(GetMousePosition(), 1, (Color){ 255, 255, 255, 255 }, 0.3f);

    return 0;
}

void draw_game(const ApplicationSettings* settings, RenderTexture screen) {

    Vector2 ui_dim = (Vector2){ (float)screen.texture.width, (float)screen.texture.height };
    Vector2 ui_anchor = (Vector2){ .x = 0.0f, .y = 0.0f };
    Vector2 mouse_position_normalized = (Vector2){ GetMousePosition().x, ui_dim.y - GetMousePosition().y };

    Rectangle ui_rect = (Rectangle){
        .x = ui_anchor.x,
        .y = ui_anchor.y,
        .width = ui_dim.x,
        .height = ui_dim.y
    };

    Vector2 center = {
        .x = (ui_rect.x + ui_rect.width) / 2.0f,
        .y = (ui_rect.y + ui_rect.height) / 2.0f
    };

    BeginTextureMode(screen);
    draw_background();

    DrawRectangleRec(ui_rect, COLOR_VSC_1);

    // Draw court-related objects in the subscreen [0, 0] x [ui_width, ui_height]
    draw_game_court(ui_rect);

    // TODO: debug; remove
    draw_circle_outline(mouse_position_normalized, 50.0f, ORANGE, 0.5f);

    EndTextureMode();
    DrawTextureRec(screen.texture, ui_rect, ui_anchor, WHITE);
}

void draw_chat(const ApplicationSettings* settings, const Chat* chat, const Font* font, RenderTexture screen) {

    if (chat == NULL) { B_ERROR("Passed null parameter 'chat'"); return; }
    if (font == NULL) { B_ERROR("Passed null parameter 'font'"); return; }

    Vector2 shadow_offset = { 7, 7 };
    Vector2 input_offset = { 4, 4 };
    const int input_height = 34;
    const int padding = 4;
    const int font_size = 16 * 2;

    // TODO: figure out; flip this because render textures are upside-down?
    Vector2 ui_dim = (Vector2){ (float)screen.texture.width, -(float)screen.texture.height };
    Vector2 ui_anchor = (Vector2){
        .x = ((float)settings->window_settings.window_width * (HORIZONTAL_RATIO)),
        .y = ((float)settings->window_settings.window_height * (VERTICAL_RATIO))
    };
    Vector2 mouse_position_normalized = (Vector2){ GetMousePosition().x - ui_anchor.x, GetMousePosition().y - ui_anchor.y };
    Rectangle ui_rect = (Rectangle){
        .x = ui_anchor.x,
        .y = ui_anchor.y,
        .width = ui_dim.x,
        .height = ui_dim.y
    };

    BeginTextureMode(screen);
    draw_background();

    DrawRectangle(0, 0, (int)ui_rect.width, (int)ui_rect.height, COLOR_CHAT_BACKGROUND);

    // Draw active text box
    int pb_w = (int)ui_rect.width;
    int pb_h = font_size - (padding * 1);
    int pb_x = 0;
    int pb_y = -(int)ui_rect.height - pb_h;
    DrawRectangle(pb_x, pb_y, pb_w, pb_h, COLOR_VSC_4);

    // Draw active text message
    // Display the chat hint if there is no active text
    ChatMessage hint_message = (ChatMessage){ .text = "type a message!", .text_size = 15, .userId = 0 };
    ChatMessage message = chat->active_message;
    char buffer[128] = { 0 };
    const Vector2 active_text_pos = (Vector2){ (float)(pb_x + (padding / 2)), (float)(pb_y + (padding / 2)) };
    if (chat->active_message.text_size == 0) {
        format_chat_message(buffer, hint_message);
        DrawTextEx(*font, buffer, active_text_pos, (float)font->baseSize, (float)padding, COLOR_VSC_3);
    } else {
        format_chat_message(buffer, message);
        DrawTextEx(*font, buffer, active_text_pos, (float)font->baseSize, (float)padding, COLOR_VSC_5);
    }

    // Draw the chat history
    Vector2 text_pos = (Vector2){ 0 };
    for (uint16_t index = 0; index < chat->history.num_messages; index++) {
        ChatMessage message = get_message(chat, index);
        char buffer[128] = { 0 };
        format_chat_message(buffer, message);
        text_pos = (Vector2){
            (float)(pb_x),
            (float)(pb_y - (font_size * (chat->history.num_messages - index)))
        };
        DrawTextEx(*font, buffer, text_pos, (float)font->baseSize, (float)padding, COLOR_VSC_5);
    }

    draw_circle_outline(mouse_position_normalized, 50.0f, WHITE, 0.5f);

    EndTextureMode();
    DrawTextureRec(screen.texture, ui_rect, ui_anchor, WHITE);

}

void draw_game_info(const ApplicationSettings* settings, RenderTexture screen) {

    Vector2 ui_dim = (Vector2){ (float)screen.texture.width, (float)screen.texture.height };
    Vector2 ui_anchor = (Vector2){
        .x = ((float)settings->window_settings.window_width * (HORIZONTAL_RATIO)),
        .y = 0.0f
    };
    Vector2 mouse_position_normalized = (Vector2){ GetMousePosition().x - ui_anchor.x, ui_dim.y - GetMousePosition().y + ui_anchor.y };
    Rectangle ui_rect = (Rectangle){
        .x = ui_anchor.x,
        .y = ui_anchor.y,
        .width = ui_dim.x,
        .height = ui_dim.y
    };

    Vector2 center = {
        .x = (ui_rect.x + ui_rect.width) / 2.0f,
        .y = (ui_rect.y + ui_rect.height) / 2.0f
    };

    BeginTextureMode(screen);
    draw_background();

    DrawRectangle(0, 0, (int)ui_rect.width, (int)ui_rect.height, COLOR_VSC_1);
    draw_circle_outline(mouse_position_normalized, 50.0f, GREEN, 0.5f);

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
    DrawText(TextFormat("App: %s", get_app_type(*settings)), offset_x, offset_y + (20 * i++), 20, COLOR_TEXT);
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

