#include "init.h"

WindowSettings populate_window_application_settings(void){

    WindowSettings ws;
    ws.window_width           = 1000;
    ws.window_height          = 800;
    ws.target_fps             = 120;
    ws.log_level              = LOG_FATAL;
    ws.config_flags           = FLAG_MSAA_4X_HINT;
    return ws;
}

ErrorCode init(ApplicationSettings* application_settings) {


    // TODO: Parse a settings file to populate these settings; I don't care right now, so I'll do it a different (but worse way) for now

    WindowSettings window_settings = populate_window_application_settings();

    // Initialize and configure the window
    B_INFO("Initializing Raylib 5.6 and configuring the main window");
    InitWindow(window_settings.window_width, window_settings.window_height, "Bocce");
    SetTraceLogLevel(window_settings.log_level);
    SetTargetFPS(window_settings.target_fps);
    SetConfigFlags(window_settings.config_flags); // FLAG_MSAA_4X_HINT

    // TODO: Initialize arena allocator
    // TODO: Initialize networking; determine client/server
    // TODO: Initialize audio system
    // TODO: Initialize obj-file parser
    // TODO: Initialize database

    return 0;
}

ErrorCode uninit(void){
    
    // TODO: Implement

    return 0;
}

