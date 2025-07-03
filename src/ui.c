#include "ui.h"

static uint16_t child_gap = 16;
static Clay_CornerRadius corner_radius = { 8, 8, 8, 8 };
static Clay_Padding padding = { 16, 16, 16, 16 };

void button_handle(Clay_ElementId id, Clay_PointerData pointer_data, intptr_t data) {
    if (pointer_data.state != CLAY_POINTER_DATA_PRESSED_THIS_FRAME) return;

    B_INFO("[%f] Button '%ld' Click", GetTime(), id.id);
}

void create_layout_main_menu(Clay_Context* ctx) {
    CLAY({
        .id = CLAY_ID("background"),
        .layout = {
            .layoutDirection = CLAY_LEFT_TO_RIGHT,
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
            .id = CLAY_ID("sidebar"),
            .layout = {
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .sizing = {
                    .width = CLAY_SIZING_FIXED(500),
                    .height = CLAY_SIZING_GROW(),
                },
                .childGap = child_gap,
                .padding = padding,
            },
            .backgroundColor = {45, 45, 48, 255},
            .cornerRadius = corner_radius
            }) {
            CLAY({
            .id = CLAY_ID("title-card"),
            .layout = {
                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                .sizing = {
                    .width = CLAY_SIZING_GROW(),
                    .height = CLAY_SIZING_FIXED(100),
                },
                .childGap = child_gap,
                .padding = padding,
                .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }
            },
            .backgroundColor = {67, 67, 67, 255},
            .cornerRadius = corner_radius
                }) {
                CLAY_TEXT(
                    CLAY_STRING("Bocce Main Menu"),
                    CLAY_TEXT_CONFIG({
                        .fontId = 0,
                        .fontSize = 32,
                        .textColor = {0, 122, 204, 255}
                        })
                );
            }
            CLAY({
                .id = CLAY_ID("button-join"),
                .layout = {
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .sizing = {
                        .width = CLAY_SIZING_GROW(),
                        .height = CLAY_SIZING_FIT(),
                    },
                    .childGap = child_gap,
                    .padding = padding,
                    .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }
                },
                .cornerRadius = corner_radius,
                .backgroundColor = (Clay_Hovered() ? COLOR_BUTTON_HOVER : COLOR_BUTTON),
            }) {
                Clay_OnHover(button_handle, (intptr_t)NULL);
                CLAY_TEXT(
                    CLAY_STRING("Join"),
                    CLAY_TEXT_CONFIG({
                        .fontId = 0,
                        .fontSize = 32,
                        .textColor = {0, 122, 204, 255}
                    })
                );
            }
            CLAY({
                .id = CLAY_ID("button-stats"),
                .layout = {
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .sizing = {
                        .width = CLAY_SIZING_GROW(),
                        .height = CLAY_SIZING_FIT(),
                    },
                    .childGap = child_gap,
                    .padding = padding,
                    .childAlignment = { CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER }
                },
                .cornerRadius = corner_radius,
                .backgroundColor = (Clay_Hovered() ? COLOR_BUTTON_HOVER : COLOR_BUTTON),
            }) {
                Clay_OnHover(button_handle, (intptr_t)NULL);
                CLAY_TEXT(
                    CLAY_STRING("Stats"),
                    CLAY_TEXT_CONFIG({
                        .fontId = 0,
                        .fontSize = 32,
                        .textColor = {0, 122, 204, 255}
                    })
                );
            }
        }
        if (true) {
            CLAY({
            .id = CLAY_ID("mid-menu"),
            .layout = {
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
                .sizing = {
                    .width = CLAY_SIZING_GROW(),
                    .height = CLAY_SIZING_GROW(),
                },
                .childGap = child_gap,
                .padding = padding,
            },
            .backgroundColor = {45, 45, 48, 255},
            .cornerRadius = corner_radius
            });
        }
    }
}

void create_layout_lobby(Clay_Context* ctx) {
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
        CLAY_TEXT(
            CLAY_STRING("Bocce Lobby"),
            CLAY_TEXT_CONFIG({
                .fontId = 0,
                .fontSize = 16,
                .textColor = {0, 122, 204, 255}
                })
        );
    }
}

void create_layout_game(Clay_Context* ctx) {
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
            }) {
        };
    }
}

Clay_RenderCommandArray create_layout(Clay_Context* ctx, SceneID scene_id) {

    Clay_SetCurrentContext(ctx);
    Clay_SetDebugModeEnabled(true);

    Vector2 mousePosition = GetMousePosition();
    Vector2 scrollDelta = GetMouseWheelMoveV();
    Clay_SetPointerState(
        (Clay_Vector2) {
        mousePosition.x, mousePosition.y
    },
        IsMouseButtonDown(0)
    );

    Clay_BeginLayout();

    switch (scene_id) {
    case(SCENE_MAIN): { create_layout_main_menu(ctx); break; }
    case(SCENE_LOBBY): { create_layout_lobby(ctx); break; }
    case(SCENE_GAME): { create_layout_game(ctx); break; }
    default: { break; }
    }

    return Clay_EndLayout();
}