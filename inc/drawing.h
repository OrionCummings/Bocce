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

#define HORIZONTAL_RATIO ((float)(2.0f/3.0f))
#define VERTICAL_RATIO ((float)(1.0f/2.0f))
// #define VERTICAL_RATIO ((float)(1.0f/3.0f)) // TODO: Fix this bug; it works with 0.5 for now!

#define COLOR_VSC_0 ((Color){10, 10, 15, 255})
#define COLOR_VSC_1 ((Color){30, 30, 30, 255})
#define COLOR_VSC_2 ((Color){37, 37, 38, 255})
#define COLOR_VSC_3 ((Color){45, 45, 48, 255})
#define COLOR_VSC_4 ((Color){62, 62, 66, 255})
#define COLOR_VSC_5 ((Color){0, 122, 204, 255})

#define COLOR_GAME_BACKGROUND ((Color){196, 177, 174, 255})
#define COLOR_GAME_INFO_BACKGROUND ((Color){202, 183, 162, 255})
#define COLOR_CHAT_BACKGROUND ((Color){133, 135, 134, 255})

#define COLOR_SHADOW (COLOR_VSC_0)
#define COLOR_BACKGROUND (COLOR_VSC_1)
#define COLOR_TEXT (COLOR_VSC_4)
#define COLOR_HIGHLIGHT (COLOR_VSC_5)

// https://coolors.co/palette/606c38-283618-fefae0-dda15e-bc6c25
#define COLOR_COURT_BASE_DARK ((Color){40, 54, 24, 255})
#define COLOR_COURT_BASE ((Color){96, 108, 56, 255})
#define COLOR_COURT_BEIGE ((Color){254, 250, 224, 255})
#define COLOR_COURT_WALL ((Color){221, 161, 94, 255})
#define COLOR_COURT_WALL_DARK ((Color){188, 108, 37, 255})

#define COLOR_RANDOM ((Color){(unsigned char)rand(), (unsigned char)rand(), (unsigned char)rand(), 255})

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
ErrorCode draw(const ApplicationSettings*, const GameState*, const Chat*, const Font*, RenderTexture, RenderTexture, RenderTexture, Clay_RenderCommandArray);
void draw_game(const ApplicationSettings*, RenderTexture);
void draw_game_info(const ApplicationSettings*, RenderTexture);
void draw_chat(const ApplicationSettings*, const Chat*, const Font*, RenderTexture);
void draw_background(void);
void draw_circle_outline(Vector2, float, Color, float dim_factor);
void draw_ball(const Ball);
void draw_balls(const Ball*, uint16_t);
void draw_ui_base(Rectangle);
void draw_game_court(const Rectangle ui_rect);

// TextBox
TextBox create_active_text_box(Rectangle, int);
void draw_text_box(TextBox);


// Misc
Ball generate_ball(Vector2 mouse_position);

// Debug
void draw_debug_information(const ApplicationSettings*);

// Inlines
// TODO: Fix this; does not handle non-Q4 rectangles
static inline Rectangle rect_shrink(Rectangle r, float factor) {
    return (Rectangle){r.x + factor, r.y + factor, r.width - (2 * factor), r.height - (2 * factor)};
}

#endif