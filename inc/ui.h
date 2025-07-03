#ifndef __UI_H
#define __UI_H

#include "types.h"
#include "debug.h"
#include "raylib.h"
#include "clay.h"
#include "scene.h"
#include "colors.h"

#define COLOR_BUTTON_HOVER ((Clay_Color){37, 37, 38, 255})
#define COLOR_BUTTON ((Clay_Color){67, 67, 67, 255})

Clay_RenderCommandArray create_layout(Clay_Context* ctx, SceneID id);
void create_layout_main_menu(Clay_Context* ctx);
void create_layout_lobby(Clay_Context* ctx);
void create_layout_game(Clay_Context* ctx);

#endif