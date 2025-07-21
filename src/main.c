// This removes a lot of bloat from windows header files
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <sqlite3.h>
#include "banned.h"
#include "constants.h"
#include "types.h"
#include "debug.h"
#include "init.h"
#include "drawing.h"
#include "error_codes.h"
#include "update.h"

// TODO: Reduce the scope of these variables!
static ApplicationSettings settings;
static Client client;
static Server server;
static sqlite3* database;
static GameState state;

// Debug
static int looped = 0;

// TODO: Refactor these parameters into a single "LoopState" struct or something better!
ErrorCode loop(ApplicationSettings* settings, Server* server, Client* client, GameState* state){

    if (looped++) return -1;

    B_INFO("Executing one main loop");

    // TODO: Align this with the current parameter list! Apply globally!
    if (settings == NULL) { B_ERROR("Passed null parameter 'settings'"); return EC_PASSED_NULL; }
    if (server == NULL) { B_ERROR("Passed null parameter 'server'");   return EC_PASSED_NULL; }
    if (client == NULL) { B_ERROR("Passed null parameter 'client'");   return EC_PASSED_NULL; }
    if (state == NULL) { B_ERROR("Passed null parameter 'state'");   return EC_PASSED_NULL; }

    // Update the application state
    ErrorCode ec_update = update(settings, server, client, state);
    if (ec_update){
        B_ERROR("Failed to update!");
        return ec_update;
    }

    return EC_OK;
}

int main(int argc, char** argv) {

    B_INFO("Starting Bocce application");
    B_INFO("C Version: %d (requires 202000/C23)", __STDC_VERSION__);

    // Initialize the application
    ErrorCode ec_init = init(&settings, &server, &client, &database);
    if (ec_init) {
        B_ERROR("Failed to initialize");
        B_ERROR("Exiting with error code '%d'", ec_init);
        return ec_init;
    }

    // Loop the application until it should close
    ErrorCode ec_loop = loop(&settings, &server, &client, &state);
    if (ec_loop) {
        B_ERROR("Failed to loop");
        B_ERROR("Exiting with error code '%d'", ec_loop);
        return ec_loop;
    }

    // Uninitialize the application
    ErrorCode ec_uinit = uninit(&settings, &database);
    if (ec_uinit) {
        B_ERROR("Failed to uninitialize");
        B_ERROR("Exiting with error code '%d'", ec_uinit);
        return ec_uinit;
    }

    B_INFO("Exiting program");

    return EC_OK;
}



