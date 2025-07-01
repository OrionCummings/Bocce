// This removes a lot of bloat from windows header files
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// This must appear here. Do not move it.
// Admire my house of cards from a respectful distance.
// Thanks
// xoxo
#define RAYGUI_IMPLEMENTATION (1)
#define PHYSAC_IMPLEMENTATION
#define CLAY_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sqlite3.h>
#include "raylib.h"
#include "raymath.h"
#include "raygui.h"
#include "physac.h"
#include "clay.h"
#include "clay_renderer_raylib.h"
#include "constants.h"
#include "types.h"
#include "debug.h"
#include "init.h"
#include "drawing.h"
#include "error_codes.h"
#include "update.h"
#include "chat.h"
#include "ui.h"

// TODO: Reduce the scope of these variables!
static ApplicationSettings settings;
static Client client;
static Server server;
static sqlite3* database;
static GameState state;
static Chat chat;
static Font font;

static RenderTexture screen_game;
static RenderTexture screen_game_info;
static RenderTexture screen_chat;

static Clay_Context* context;

// Debug
static int looped = 0;

ErrorCode loop(ApplicationSettings* settings, Server* server, Client* client, GameState* state, Chat* chat, Font* font, Clay_Context** context){

    if (looped++) return -1;

    B_INFO("Executing one main loop");

    // TODO: Align this with the current parameter list! Apply globally!
    if (settings == NULL) { B_ERROR("Passed null parameter 'settings'"); return EC_PASSED_NULL; }
    if (server == NULL) { B_ERROR("Passed null parameter 'server'");   return EC_PASSED_NULL; }
    if (client == NULL) { B_ERROR("Passed null parameter 'client'");   return EC_PASSED_NULL; }
    if (state == NULL) { B_ERROR("Passed null parameter 'state'");   return EC_PASSED_NULL; }
    if (context == NULL) { B_ERROR("Passed null parameter 'context'");   return EC_PASSED_NULL; }
    if (*context == NULL) { B_ERROR("Passed null parameter '*context'");   return EC_PASSED_NULL; }

    {
        // TODO: Pull this stuff out of loop(). Why is this re-run every loop????
        int screen_game_width = (int)((float)settings->window_settings.window_width * HORIZONTAL_RATIO);
        int screen_game_height = (int)(settings->window_settings.window_height);

        int screen_chat_width = (int)((float)settings->window_settings.window_width * (1.0f - HORIZONTAL_RATIO)) + 1; // +1 to account for float rounding!
        int screen_chat_height = (int)((float)settings->window_settings.window_height * (1.0f - VERTICAL_RATIO));

        int screen_game_info_width = (int)((float)settings->window_settings.window_width * (1.0f - HORIZONTAL_RATIO)) + 1; // +1 to account for float rounding!
        int screen_game_info_height = (int)((float)settings->window_settings.window_height * (VERTICAL_RATIO));

        screen_game = LoadRenderTexture(screen_game_width, screen_game_height);
        screen_game_info = LoadRenderTexture(screen_game_info_width, screen_game_info_height);
        screen_chat = LoadRenderTexture(screen_chat_width, screen_chat_height);
    }

    if (should_have_window(*settings)) {

        // If we have not told the window to close, keep going!
        while (!WindowShouldClose()) {
            
            // Update the layout dimensions
            Clay_SetLayoutDimensions((Clay_Dimensions) { (float)GetScreenWidth(), (float)GetScreenHeight() });
            Clay_RenderCommandArray render_commands = create_layout(*context);

            // Update the application state
            ErrorCode ec_update = update(settings, server, client, state, chat);
            if (ec_update){
                B_ERROR("Failed to update!");
                return ec_update;
            }

            // Enable drawing mode (raylib)
            BeginDrawing();

            // Draw to the screen buffer; must be in draw mode!
            ErrorCode ec_draw = draw(settings, state, chat, font, screen_game, screen_game_info, screen_chat, render_commands);
            if (ec_draw){
                B_ERROR("Failed to draw!");
                return ec_draw;
            }

            // Disable drawing mode (raylib)
            EndDrawing();
        }

    }

    {
        // TODO: Pull out!
        UnloadRenderTexture(screen_game);
    }

    return EC_OK;
}

int main(int argc, char** argv) {

    B_INFO("Starting Bocce application");
    B_INFO("C Version: %d (requires 202000/C23)", __STDC_VERSION__);

    // Initialize the application
    ErrorCode ec_init = init(&settings, &server, &client, &database, &font, &context);
    if (ec_init) {
        B_ERROR("Failed to initialize");
        B_ERROR("Exiting with error code '%d'", ec_init);
        return ec_init;
    }

    // Loop the application until it should close
    ErrorCode ec_loop = loop(&settings, &server, &client, &state, &chat, &font, &context);
    if (ec_loop) {
        B_ERROR("Failed to loop");
        B_ERROR("Exiting with error code '%d'", ec_loop);
        return ec_loop;
    }

    // Uninitialize the application
    ErrorCode ec_uinit = uninit(&settings, &database, &font);
    if (ec_uinit) {
        B_ERROR("Failed to uninitialize");
        B_ERROR("Exiting with error code '%d'", ec_uinit);
        return ec_uinit;
    }

    B_INFO("Exiting program");

    return EC_OK;
}



