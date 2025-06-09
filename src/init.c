#include "init.h"

ErrorCode init(ApplicationSettings* application_settings) {


    // TODO: Parse a settings file to populate these settings; I don't care right now, so I'll do it a different (but worse way) for now

    // Apply the default config (release invariants) and then parse the config.toml (release variants)
    apply_default_config(application_settings);
    // parse_config(application_settings);
    // print_application_settings(*application_settings);

    // Initialize and configure the window
    B_INFO("Initializing Raylib 5.6 and configuring the main window");
    WindowSettings ws = application_settings->window_settings;
    SetConfigFlags(ws.config_flags); // FLAG_MSAA_4X_HINT
    InitWindow(ws.window_width, ws.window_height, "Bocce");
    SetTraceLogLevel(ws.log_level);
    SetTargetFPS(ws.target_fps);

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


ErrorCode apply_default_config(ApplicationSettings* settings) {

    // Apply default window settings
    settings->window_settings.window_width = 1000;
    settings->window_settings.window_height = 800;
    settings->window_settings.fullscreen = false;
    memcpy_s(settings->window_settings.window_title, 256, "Multiplayer Bocce Game", strlen("Multiplayer Bocce Game"));
    settings->window_settings.config_flags = FLAG_MSAA_4X_HINT;
    settings->window_settings.log_level = LOG_ALL;
    settings->window_settings.target_fps = 120;

    // Apply default client settings
    settings->client_settings.major_version = -1;
    settings->client_settings.minor_version = -1;
    settings->client_settings.patch_version = -1;
    memcpy_s(settings->client_settings.server_ip, 16, "127.0.0.1", strlen("127.0.0.1"));
    settings->client_settings.server_port = 57283;

    // Apply default server settings
    settings->server_settings.major_version = -1;
    settings->server_settings.minor_version = -1;
    settings->server_settings.patch_version = -1;
    settings->server_settings.max_players = 16;

    return 0;
}

