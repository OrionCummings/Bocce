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
#include "scene.h"

// TODO: Reduce the scope of these variables!
static ApplicationSettings settings;
static Client client;
static Server server;
static sqlite3* database;
static GameState state;
static Chat chat;
static Font fonts[3];

static SceneID scene_current = 0;

static Clay_Context* context;

// Debug
static int looped = 0;

// TODO: Refactor these parameters into a single "LoopState" struct or something better!
ErrorCode loop(ApplicationSettings* settings, Server* server, Client* client, GameState* state, Chat* chat, Font* fonts, Clay_Context** context, SceneID* scene_current){

    if (looped++) return -1;

    B_INFO("Executing one main loop");

    // TODO: Align this with the current parameter list! Apply globally!
    if (settings == NULL) { B_ERROR("Passed null parameter 'settings'"); return EC_PASSED_NULL; }
    if (server == NULL) { B_ERROR("Passed null parameter 'server'");   return EC_PASSED_NULL; }
    if (client == NULL) { B_ERROR("Passed null parameter 'client'");   return EC_PASSED_NULL; }
    if (state == NULL) { B_ERROR("Passed null parameter 'state'");   return EC_PASSED_NULL; }
    if (context == NULL) { B_ERROR("Passed null parameter 'context'");   return EC_PASSED_NULL; }
    if (*context == NULL) { B_ERROR("Passed null parameter '*context'");   return EC_PASSED_NULL; }
    if (scene_current == NULL) { B_ERROR("Passed null parameter 'scene_current'");   return EC_PASSED_NULL; }

    if (is_server(*settings)) {

        // Update the server
        ErrorCode ec_update_server = update_server(server);
        if (ec_update_server){
            B_ERROR("Failed to update server!");
            return ec_update_server;
        }
    }

    if (is_client(*settings)) {

        // If we have not told the window to close, keep going!
        while (!WindowShouldClose()) {

            // Update the layout dimensions
            Clay_SetLayoutDimensions((Clay_Dimensions) { (float)GetScreenWidth(), (float)GetScreenHeight() });
            Clay_RenderCommandArray render_commands = create_layout(*context, *scene_current);

            // Update the client
            ErrorCode ec_update_client = update_client(client, scene_current);
            if (ec_update_client){
                B_ERROR("Failed to update client!");
                return ec_update_client;
            }

            // Enable drawing mode (raylib)
            BeginDrawing();

            // Draw to the screen buffer; must be in draw mode!
            ErrorCode ec_draw = draw(settings, state, chat, fonts, render_commands);
            if (ec_draw){
                B_ERROR("Failed to draw!");
                return ec_draw;
            }

            // Disable drawing mode (raylib)
            EndDrawing();
        }

    }

    return EC_OK;
}

int main(int argc, char** argv) {

    B_INFO("Starting Bocce application");
    B_INFO("C Version: %d (requires 202000/C23)", __STDC_VERSION__);

    // Initialize the application
    ErrorCode ec_init = init(&settings, &server, &client, &database, fonts, &context);
    if (ec_init) {
        B_ERROR("Failed to initialize");
        B_ERROR("Exiting with error code '%d'", ec_init);
        return ec_init;
    }

    // Loop the application until it should close
    ErrorCode ec_loop = loop(&settings, &server, &client, &state, &chat, fonts, &context, &scene_current);
    if (ec_loop) {
        B_ERROR("Failed to loop");
        B_ERROR("Exiting with error code '%d'", ec_loop);
        return ec_loop;
    }

    // Uninitialize the application
    ErrorCode ec_uinit = uninit(&settings, &server, &client, &database, fonts);
    if (ec_uinit) {
        B_ERROR("Failed to uninitialize");
        B_ERROR("Exiting with error code '%d'", ec_uinit);
        return ec_uinit;
    }

    B_INFO("Exiting program");

    return EC_OK;
}



