#include "drawing.h"

ErrorCode draw(const ApplicationSettings* settings, const GameState* state) {

    if (settings == NULL) { B_ERROR("Passed null parameter 'settings'"); return EC_PASSED_NULL; }
    if (state == NULL) { B_ERROR("Passed null parameter 'state'"); return EC_PASSED_NULL; }

    return EC_OK;
}

void draw_background() {



}