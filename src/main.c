// This removes a lot of bloat from windows header files
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "constants.h"
#include "types.h"
#include "debug.h"
#include "init.h"
#include "drawing.h"
#include "raylib.h"

int update(void){
    global_time += dt;
    return 0;
}

int main(int argc, char** argv) {

    B_INFO("Starting Bocce application");
    B_INFO("C Version: %d (requires 202000)", __STDC_VERSION__);
    
    // Initialize the application
    ApplicationSettings settings;
    ErrorCode ec_init = init(&settings);
        if(ec_init) {
        B_ERROR("Failed to initialize!");
    }

    // If we have not told the window to close, keep going!
    while (!WindowShouldClose())
    {
        // Update the application state
        ErrorCode ec_update = update();
        if (ec_update){
            B_ERROR("Failed to update!");
        }

        // Enable drawing mode (raylib)
        BeginDrawing();
        
        // Draw to the screen buffer; must be in draw mode!
        ErrorCode ec_draw = draw();
        if (ec_draw){
            B_ERROR("Failed to draw!");
        }
        
        // Disable drawing mode (raylib)
        EndDrawing();
    }

    // Close the window
    CloseWindow();

    // Uninitialize the application
    ErrorCode ec_uinit = uninit();
    if (ec_uinit) {
        B_ERROR("Failed to uninitialize!");
    }

    B_INFO("Exiting program");

    return 0;
}