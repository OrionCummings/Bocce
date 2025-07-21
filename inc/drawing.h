#ifndef __DRAWING_H
#define __DRAWING_H

#include "types.h"
#include "debug.h"
#include "error_codes.h"
#include "networking.h"
#include "colors.h"

// Colors
Color dim_color(const Color, float);

// Drawing
ErrorCode draw(const ApplicationSettings*, const GameState*);
void draw_game(const ApplicationSettings*);
void draw_game_info(const ApplicationSettings*);
void draw_background(void);
void draw_circle_outline(V2i, float, Color, float dim_factor);
void draw_ball(const BocceBall);
void draw_balls(const BocceBall*, uint16_t);

// Misc
BocceBall generate_ball(V2i mouse_position);

// Debug
void draw_debug_information(const ApplicationSettings*);

#endif