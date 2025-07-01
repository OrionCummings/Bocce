#include "ui.h"

Clay_RenderCommandArray create_layout(Clay_Context* ctx) {

    Clay_SetCurrentContext(ctx);

    Clay_BeginLayout();

    CLAY({
        .id = CLAY_ID("main"),
        .layout = {
            .sizing = {
                .width = CLAY_SIZING_GROW(),
                .height = CLAY_SIZING_GROW(),
            }
        },
        .backgroundColor = {255, 0, 0, 255}
        });

    return Clay_EndLayout();
}