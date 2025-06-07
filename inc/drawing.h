#ifndef __DRAWING_H
#define __DRAWING_H

#include "raylib.h"

#define COLOR_BACKGROUND ((Color){37, 37, 40, 255})

// Colors
Color dim_color(const Color, float);

// Drawing
void draw_background(void);
void draw_circle_outline(Vector2, float, Color);
int draw(void);

#endif