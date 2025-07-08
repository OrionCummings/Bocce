#ifndef __DRAWING_H
#define __DRAWING_H

#include "types.h"
#include "debug.h"
#include "raygui.h"
#include "raylib.h"
#include "error_codes.h"
#include "networking.h"
#include "chat.h"
#include "physac.h"
#include "clay.h"
#include "clay_renderer_raylib.h"
#include "colors.h"

#define HORIZONTAL_RATIO ((float)(2.0f/3.0f))
#define VERTICAL_RATIO ((float)(1.0f/2.0f))
// #define VERTICAL_RATIO ((float)(1.0f/3.0f)) // TODO: Fix this bug; it works with 0.5 for now!

#define TEXT_BOX_EMPTY_TEXT_MAX_LENGTH (128)

#define origin ((Vector2){0.0f, 0.0f})

static const float padding = 10.0f;

typedef struct TextBox {
    Rectangle bounds;
    bool selected;
    char empty_text[128];
} TextBox;

typedef struct UIManager {
    int selected_id;
} UIManager;

// Colors
Color dim_color(const Color, float);

// Drawing
ErrorCode draw(const ApplicationSettings*, const GameState*, const Chat*, Font*, Clay_RenderCommandArray);
void draw_game(const ApplicationSettings*, RenderTexture);
void draw_game_info(const ApplicationSettings*, RenderTexture);
void draw_chat(const ApplicationSettings*, const Chat*, const Font*, RenderTexture);
void draw_background(void);
void draw_circle_outline(Vector2, float, Color, float dim_factor);
void draw_ball(const BocceBall);
void draw_balls(const BocceBall*, uint16_t);
void draw_ui_base(Rectangle);
void draw_game_court(const Rectangle ui_rect);

// TextBox
TextBox create_active_text_box(Rectangle, int);
void draw_text_box(TextBox);


// Misc
BocceBall generate_ball(Vector2 mouse_position);

// Debug
void draw_debug_information(const ApplicationSettings*);

// Inlines
// TODO: Fix this; does not handle non-Q4 rectangles
static inline Rectangle rect_shrink(Rectangle r, float factor) {
    return (Rectangle){r.x + factor, r.y + factor, r.width - (2 * factor), r.height - (2 * factor)};
}

#endif