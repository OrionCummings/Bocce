#include "init.h"

ErrorCode init(ApplicationSettings* application_settings) {

    // Apply the default config and then parse the config.toml
    apply_default_application_config(application_settings);
    parse_config(application_settings);

    ErrorCode ec_init_window = init_window(application_settings);
    if (ec_init_window) {
        B_ERROR("Failed to initialize window");
    }

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

ErrorCode init_window(ApplicationSettings* application_settings) {

    // Initialize and configure the window
    B_INFO("Initializing Raylib 5.6 and configuring the main window");
    WindowSettings ws = application_settings->window_settings;
    SetConfigFlags(ws.config_flags); // FLAG_MSAA_4X_HINT
    SetTraceLogLevel(ws.log_level);
    SetTargetFPS(ws.target_fps);
    InitWindow(ws.window_width, ws.window_height, "Bocce");

    return 0;
}

ErrorCode apply_default_application_config(ApplicationSettings* settings) {

    // Apply default window settings
    settings->window_settings.window_width = 1000;
    settings->window_settings.window_height = 800;
    settings->window_settings.fullscreen = false;
    memcpy_s(settings->window_settings.window_title, 256, "Multiplayer Bocce Game", strlen("Multiplayer Bocce Game"));
    settings->window_settings.config_flags = FLAG_MSAA_4X_HINT;
    settings->window_settings.log_level = LOG_NONE;
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

