#include "ui.h"

Clay_RenderCommandArray create_layout(Clay_Context* ctx) {

    Clay_SetCurrentContext(ctx);

    uint16_t child_gap = 16;
    Clay_CornerRadius corner_radius = { 8, 8, 8, 8 };
    Clay_Padding padding = { 16, 16, 16, 16 };

    Clay_BeginLayout();

    CLAY({
        .id = CLAY_ID("background"),
        .layout = {
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
            .sizing = {
                .width = CLAY_SIZING_GROW(),
                .height = CLAY_SIZING_GROW(),
            },
            .childGap = child_gap,
            .padding = padding
        },
        .backgroundColor = {30, 30, 30, 255},
        }) {
        CLAY({
            .id = CLAY_ID("content"),
            .layout = {
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .sizing = {
                    .width = CLAY_SIZING_GROW(),
                    .height = CLAY_SIZING_GROW(),
                },
                .padding = padding,
                .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER}
            },
            .backgroundColor = {60, 60, 60, 255},
            .cornerRadius = corner_radius
        }) {
            CLAY({
                .id = CLAY_ID("court"),
                .layout = {
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .sizing = {
                        .width = CLAY_SIZING_PERCENT(0.85f),
                        .height = CLAY_SIZING_PERCENT(0.85f),
                    },
                    .padding = padding
                },
                .backgroundColor = {125, 125, 125, 255},
                .cornerRadius = corner_radius
                });
        };
        CLAY({
            .id = CLAY_ID("footer"),
            .layout = {
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .sizing = {
                    .width = CLAY_SIZING_GROW(),
                    .height = CLAY_SIZING_FIXED(100),
                },
                .padding = padding
            },
            .backgroundColor = {45, 45, 48, 255},
            .cornerRadius = corner_radius
        }) {};
    }

    return Clay_EndLayout();
}