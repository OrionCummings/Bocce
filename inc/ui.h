#ifndef __UI_H
#define __UI_H

#include "raylib.h"

#define BUTTON_TEXT_MAX_LENGTH (64)

typedef struct Button {
    Rectangle bounds;
    bool selected;
    char text[BUTTON_TEXT_MAX_LENGTH];
} Button;

bool clicked(Button, Vector2);
bool is_selected(const Button*);

#endif