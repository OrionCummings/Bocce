// This removes a lot of bloat from windows header files
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// This must appear here. Do not move it.
// Admire my house of cards from a respectful distance.
// Thanks
// xoxo
#define RAYGUI_IMPLEMENTATION (1)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sqlite3.h>
#include "raylib.h"
#include "raygui.h"
#include "constants.h"
#include "types.h"
#include "debug.h"
#include "init.h"
#include "drawing.h"
#include "error_codes.h"
#include "update.h"
#include "chat.h"

static ApplicationSettings settings;
static Client client;
static Server server;
static sqlite3* database;
static GameState state;
static Chat chat;
static Font font;

// Debug
static int looped = 0;

ErrorCode loop(ApplicationSettings* settings, Server* server, Client* client, GameState* state, Chat* chat, Font* font){

    if (looped++) return -1;

    B_INFO("Executing one main loop");

    if (should_have_window(*settings)) {

        // If we have not told the window to close, keep going!
        while (!WindowShouldClose()) {

            if (settings == NULL) { B_ERROR("Passed null parameter 'settings'"); return EC_PASSED_NULL; }
            if (server == NULL) { B_ERROR("Passed null parameter 'server'");   return EC_PASSED_NULL; }
            if (client == NULL) { B_ERROR("Passed null parameter 'client'");   return EC_PASSED_NULL; }
            if (state == NULL) { B_ERROR("Passed null parameter 'state'");   return EC_PASSED_NULL; }

            // Update the application state
            ErrorCode ec_update = update(settings, server, client, state, chat);
            if (ec_update){
                B_ERROR("Failed to update!");
                return ec_update;
            }

            // Enable drawing mode (raylib)
            BeginDrawing();

            // Draw to the screen buffer; must be in draw mode!
            ErrorCode ec_draw = draw(settings, state, chat, font);
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
    ErrorCode ec_init = init(&settings, &server, &client, &database, &font);
    if (ec_init) {
        B_ERROR("Failed to initialize");
        B_ERROR("Exiting with error code '%d'", ec_init);
        return ec_init;
    }

    ErrorCode ec_loop = loop(&settings, &server, &client, &state, &chat, &font);
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



