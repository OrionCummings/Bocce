#include "init.h"

static TcsSocket client_socket;

ErrorCode init(ApplicationSettings* application_settings) {

    // Set all memory to zero
    memset(application_settings, 0, sizeof(*application_settings));

    // Apply the default config and then parse the config.toml
    ErrorCode ec_apply_default_application_config = apply_default_application_config(application_settings);
    if (ec_apply_default_application_config) {
        B_ERROR("Failed to apply default application config");
        return ec_apply_default_application_config;
    }

    ErrorCode ec_parse_config = parse_config(application_settings);
    if (ec_parse_config) {
        B_ERROR("Failed to parse application config");
        return ec_parse_config;
    }

    ErrorCode ec_init_window = init_window(application_settings);
    if (ec_init_window) {
        B_ERROR("Failed to initialize window");
        return ec_init_window;
    }

    ErrorCode ec_init_networking = init_networking(application_settings);
    if (ec_init_networking) {
        B_ERROR("Failed to initialize networking");
        return ec_init_networking;
    }

    // TODO: Initialize arena allocator
    // TODO: Initialize audio system
    // TODO: Initialize obj-file parser
    // TODO: Initialize database

    return EC_OK;
}

ErrorCode init_window(ApplicationSettings* application_settings) {

    // Initialize and configure the window
    B_INFO("Initializing Raylib 5.6 and configuring the main window");
    WindowSettings ws = application_settings->window_settings;
    SetConfigFlags(ws.config_flags); // FLAG_MSAA_4X_HINT
    SetTraceLogLevel(ws.log_level);
    SetTargetFPS(ws.target_fps);
    InitWindow(ws.window_width, ws.window_height, ws.window_title);

    return EC_OK;
}

ErrorCode init_networking(ApplicationSettings* application_settings) {

    // Determine if this is the client or the server
    // TODO: Make this dynamic!
    bool is_client = true;

    if (is_client) {
        ErrorCode ec_init_networking_client = init_networking_client(application_settings->client_settings);
        if (ec_init_networking_client) {
            B_ERROR("Failed to initialize networking client");
            return ec_init_networking_client;
        }
    } else {
        ErrorCode ec_init_networking_server = init_networking_server(application_settings->server_settings);
        if (ec_init_networking_server) {
            B_ERROR("Failed to initialize networking server");
            return ec_init_networking_server;
        }
    }

    return 0;
}

ErrorCode init_networking_client(ClientSettings settings) {

    // TODO: Remove this and add a graceful check for the server!
    B_ERROR("Ensure the server is active before trying to run! -Orion");
    return -1;

    if (tcs_lib_init() != TCS_SUCCESS) {
        B_ERROR("Client failed to initialize tinycsockets");
        return EC_TCS_CLIENT_INIT_FAILURE;
    }

    client_socket = TCS_NULLSOCKET;
    if (tcs_create(&client_socket, TCS_TYPE_TCP_IP4) != TCS_SUCCESS) {
        B_ERROR("Client failed to create socket");
        return EC_TCS_CLIENT_SOCKET_CREATE_FAILURE;
    }

    const char* ip = settings.server_ip;
    const uint16_t port = (uint16_t)settings.server_port;

    if (tcs_connect(client_socket, ip, port) != TCS_SUCCESS) {
        B_ERROR("Client failed to connect to server");
        return EC_TCS_CLIENT_CONNECTION_FAILURE;
    }

    char buffer[] = "bocce client connecting";
    size_t size = sizeof(buffer);
    const uint8_t* cbuffer = (const uint8_t*)buffer;
    // ErrorCode ec_send = send(client_socket, cbuffer, size);
    // if (ec_send) {
    if (tcs_send(client_socket, cbuffer, size, TCS_MSG_SENDALL, NULL) != TCS_SUCCESS) {
        B_ERROR("Client failed to send data to the server");
        return 342;
    }

    uint8_t recv_buffer[1024];
    size_t recv_size = sizeof(recv_buffer) - sizeof('\0');
    size_t bytes_received = 0;
    if (tcs_receive(client_socket, recv_buffer, recv_size, TCS_NO_FLAGS, &bytes_received) != TCS_SUCCESS) {
        B_ERROR("Client failed to receive data from the server");
        return EC_TCS_CLIENT_RECEPTION_FAILURE;
    }

    recv_buffer[bytes_received] = '\0';
    B_INFO("Received: '%s'", recv_buffer);

    return EC_OK;
}

ErrorCode init_networking_server(ServerSettings settings) {

    return EC_OK;
}

// Uninitialzation functions
ErrorCode uninit(void){

    B_INFO("Uninitializing network");
    ErrorCode ec_uninit_networking = uninit_networking();
    if (ec_uninit_networking) {
        B_ERROR("Failed to uninitialize networking");
        return ec_uninit_networking;
    }

    return EC_OK;
}

ErrorCode uninit_networking() {

    B_INFO("Client shutting down socket");
    if (tcs_shutdown(client_socket, TCS_SD_BOTH) != TCS_SUCCESS){
        B_ERROR("Client failed to shutdown socket");
        return EC_TCS_CLIENT_SOCKET_SHUTDOWN_FAILURE;
    }
    
    B_INFO("Client closing socket");
    if (tcs_destroy(&client_socket) != TCS_SUCCESS) {
        B_ERROR("Client failed to close socket");
        return EC_TCS_CLIENT_SOCKET_CLOSE_FAILURE;
    }
    
    B_INFO("Client freeing tinycsocket");
    if (tcs_lib_free() != TCS_SUCCESS) {
        B_ERROR("Client failed to free tinycsocket");
        return EC_TCS_CLIENT_SOCKET_FREE_TINYCSOCKET_FAILURE;
    }

    return EC_OK;
}

// Helper functions
ErrorCode apply_default_application_config(ApplicationSettings* settings) {

    // Apply default window settings
    settings->window_settings.window_width = 1000;
    settings->window_settings.window_height = 800;
    settings->window_settings.fullscreen = false;
    memset(settings->window_settings.window_title, 0, 256);
    memcpy_s(settings->window_settings.window_title, 256, "Multiplayer Bocce Game", strlen("Multiplayer Bocce Game"));
    settings->window_settings.config_flags = FLAG_MSAA_4X_HINT;
    settings->window_settings.log_level = LOG_NONE;
    settings->window_settings.target_fps = 120;
    
    // Apply default client settings
    settings->client_settings.major_version = -1;
    settings->client_settings.minor_version = -1;
    settings->client_settings.patch_version = -1;
    memset(settings->client_settings.server_ip, 0, 16);
    memcpy_s(settings->client_settings.server_ip, 16, "127.0.0.1", strlen("127.0.0.1"));
    settings->client_settings.server_port = 57283;

    // Apply default server settings
    settings->server_settings.major_version = -1;
    settings->server_settings.minor_version = -1;
    settings->server_settings.patch_version = -1;
    settings->server_settings.max_players = 16;

    return EC_OK;
}

