#ifndef __DRAWING_H
#define __DRAWING_H

#include "types.h"
#include "debug.h"
#include "raylib.h"
#include "error_codes.h"
#include "networking.h"

#define COLOR_VSC_1 ((Color){30, 30, 30, 255})
#define COLOR_VSC_2 ((Color){37, 37, 38, 255})
#define COLOR_VSC_3 ((Color){45, 45, 48, 255})
#define COLOR_VSC_4 ((Color){62, 62, 66, 255})
#define COLOR_VSC_5 ((Color){0, 122, 204, 255})

#define COLOR_BACKGROUND (COLOR_VSC_1)
#define COLOR_TEXT (COLOR_VSC_4)
#define COLOR_HIGHLIGHT (COLOR_VSC_5)

// Colors
Color dim_color(const Color, float);

// Drawing
void draw_background(void);
void draw_circle_outline(Vector2, float, Color);
ErrorCode draw(const ApplicationSettings*);

// Debug
void draw_debug_information(const ApplicationSettings*);

#endif