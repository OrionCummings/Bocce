#include "ui.h"

bool clicked(Button b, Vector2 p) {
    return CheckCollisionPointRec(p, b.bounds);
}

bool is_selected(const Button* b) {
    return b->selected;
}
