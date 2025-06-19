#ifndef __DRAWING_H
#define __DRAWING_H

#include "types.h"
#include "debug.h"
#include "raygui.h"
#include "raylib.h"
#include "error_codes.h"
#include "networking.h"
#include "chat.h"

#define HORIZONTAL_RATIO (2.0f/3.0f)
#define VERTICAL_RATIO (1.0f/2.0f)

#define COLOR_VSC_1 ((Color){30, 30, 30, 255})
#define COLOR_VSC_2 ((Color){37, 37, 38, 255})
#define COLOR_VSC_3 ((Color){45, 45, 48, 255})
#define COLOR_VSC_4 ((Color){62, 62, 66, 255})
#define COLOR_VSC_5 ((Color){0, 122, 204, 255})

#define COLOR_BACKGROUND (COLOR_VSC_1)
#define COLOR_TEXT (COLOR_VSC_4)
#define COLOR_HIGHLIGHT (COLOR_VSC_5)

// https://coolors.co/palette/606c38-283618-fefae0-dda15e-bc6c25
#define COLOR_COURT_BASE_DARK ((Color){40, 54, 24, 255})
#define COLOR_COURT_BASE ((Color){96, 108, 56, 255})
#define COLOR_COURT_BEIGE ((Color){254, 250, 224, 255})
#define COLOR_COURT_WALL ((Color){221, 161, 94, 255})
#define COLOR_COURT_WALL_DARK ((Color){188, 108, 37, 255})

#define origin ((Vector2){0.0f, 0.0f})
static const float padding = 10.0f;

// Colors
Color dim_color(const Color, float);

// Drawing
ErrorCode draw(const ApplicationSettings*, const GameState*, const Chat*, const Font*);
void draw_court(const WindowSettings);
void draw_game_info(const WindowSettings);
void draw_chat(const WindowSettings, Vector2, const Vector2, const Chat*, const Font*);
void draw_background(void);
void draw_circle_outline(Vector2, float, Color, float dim_factor);
void draw_ball(const Ball);
void draw_balls(const Ball*, uint16_t);

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