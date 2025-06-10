#include "init.h"

static TcsSocket client_socket;
static TcsSocket server_socket;
static TcsSocket listen_socket;

ErrorCode init(ApplicationSettings* application_settings) {

    // Set all memory to zero
    memset(application_settings, 0, sizeof(*application_settings));

    // Apply the default config and then parse the config.toml
    // TODO: This may not be good as it populates the ApplicationSettings regardless of the ApplicationMode
    ErrorCode ec_apply_default_application_config = apply_default_application_config(application_settings);
    if (ec_apply_default_application_config) {
        B_ERROR("Failed to apply default application config");
        return ec_apply_default_application_config;
    }

    // Parse the config file
    ErrorCode ec_parse_config = parse_config(application_settings);
    if (ec_parse_config) {
        B_ERROR("Failed to parse application config");
        return ec_parse_config;
    }

    ErrorCode ec_init_client;
    ErrorCode ec_init_server;
    switch (application_settings->application_mode) {
    case AM_CLIENT:
        ec_init_client = init_client(application_settings);
        if (ec_init_client) {
            B_ERROR("Failed to initialize client");
            return ec_init_client;
        }
        break;

    case AM_SERVER:
        ec_init_server = init_server(application_settings);
        if (ec_init_server) {
            B_ERROR("Failed to initialize server");
            return ec_init_server;
        }
        break;

    case AM_DUAL:
        ec_init_server = init_server(application_settings);
        if (ec_init_server) {
            B_ERROR("Failed to initialize server");
            return ec_init_server;
        }
        ec_init_client = init_client(application_settings);
        if (ec_init_client) {
            B_ERROR("Failed to initialize client");
            return ec_init_client;
        }
        break;

    case AM_UNKNOWN:
    default:
        B_ERROR("Unknown application mode '%d'", application_settings->application_mode);
        return EC_UNKNOWN_APPLICATION_MODE;
        break;
    }

    // Initialize networking
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

ErrorCode init_client(ApplicationSettings* application_settings) {

    // Initialize the window
    ErrorCode ec_init_window = init_window(application_settings);
    if (ec_init_window) {
        B_ERROR("Failed to initialize window");
        return ec_init_window;
    }

    return EC_OK;
}

ErrorCode init_server(ApplicationSettings* application_settings) {

    // TODO: Implement

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
    if (is_server(*application_settings)) {
        ErrorCode ec_init_networking_server = init_networking_server(application_settings->server_settings);
        if (ec_init_networking_server) {
            B_ERROR("Failed to initialize networking server");
            return ec_init_networking_server;
        }
    }

    if (is_client(*application_settings)) {
        ErrorCode ec_init_networking_client = init_networking_client(application_settings->client_settings);
        if (ec_init_networking_client) {
            B_ERROR("Failed to initialize networking client");
            return ec_init_networking_client;
        }
    }

    return EC_OK;
}

ErrorCode init_networking_client(ClientSettings settings) {

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

    // NetworkCommand nc = {
    //     .type = NCT_CONNECT,
    // };

    size_t n = 32; // TODO: Magic number!
    char buffer[n]; // TODO: this lowkey leaks memory
    snprintf(buffer, n, "CN.%d", settings.id);

    size_t size = sizeof(buffer);
    const uint8_t* cbuffer = (const uint8_t*)buffer;
    ErrorCode ec_send = send_data(client_socket, cbuffer, size);
    if (ec_send) {
    // if (tcs_send(client_socket, cbuffer, size, TCS_MSG_SENDALL, NULL) != TCS_SUCCESS) {
        B_ERROR("Client failed to send data to the server");
        return ec_send;
        // return EC_TCS_CLIENT_SEND_FAILURE;
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

    if (tcs_lib_init() != TCS_SUCCESS) {
        B_ERROR("Server failed to initialize tinycsockets");
        return EC_TCS_SERVER_INIT_FAILURE;
    }

    server_socket = TCS_NULLSOCKET;
    listen_socket = TCS_NULLSOCKET;
    if (tcs_create(&listen_socket, TCS_TYPE_TCP_IP4) != TCS_SUCCESS) {
        B_ERROR("Server failed to create listening socket");
        return EC_TCS_LISTEN_SOCKET_CREATE_FAILURE;
    }

    uint16_t listen_port = (uint16_t)(settings.port);
    if (tcs_listen_to(listen_socket, listen_port)) {
        B_ERROR("Server failed to listen on port '%d'", listen_port);
        return EC_TCS_LISTEN_SOCKET_LISTEN_FAILURE;
    }

    if (tcs_accept(listen_socket, &server_socket, NULL)) {
        B_ERROR("Server failed to accept connection");
        return EC_TCS_LISTEN_SOCKET_CONNCTION_ACCEPTANCE_FAILURE;
    }

    if (tcs_destroy(&listen_socket) != TCS_SUCCESS) {
        B_ERROR("Server failed to destroy listening socket");
        return EC_TCS_LISTEN_SOCKET_DESTRUCTION_FAILURE;
    }

    uint8_t recv_buffer[1024]; // TODO: Remove magic numbers!
    size_t recv_size = sizeof(recv_buffer) - sizeof('\0');
    size_t bytes_received = 0;
    if (tcs_receive(server_socket, recv_buffer, recv_size, TCS_NO_FLAGS, &bytes_received) != TCS_SUCCESS) {
        B_ERROR("Server failed to receive data from client");
        return EC_TCS_SERVER_SOCKET_RECEPTION_FAILURE;
    }

    recv_buffer[bytes_received] = '\0';
    printf("received: %s\n", recv_buffer);

    char msg[] = "CN.ACK\n";
    if (tcs_send(server_socket, (const uint8_t*)msg, sizeof(msg), TCS_MSG_SENDALL, NULL) != TCS_SUCCESS) {
        B_ERROR("Server failed to send data to client");
        return EC_TCS_SERVER_DATA_TRANSMISSION_FAILURE;
    }

    return EC_OK;
}

// Uninitialzation functions
ErrorCode uninit(ApplicationSettings* application_settings){

    B_INFO("Uninitializing network");
    ErrorCode ec_uninit_networking = uninit_networking(application_settings);
    if (ec_uninit_networking) {
        B_ERROR("Failed to uninitialize networking");
        return ec_uninit_networking;
    }

    return EC_OK;
}

ErrorCode uninit_networking(ApplicationSettings* application_settings) {

    if (application_settings == NULL) {
        B_ERROR("Passed NULL parameter 'application_settings'");
        return EC_UNINIT_NETWORKING_PASSED_NULL;
    }

    if (is_server(*application_settings)) {

        B_INFO("Server shutting down server socket");
        if (tcs_shutdown(server_socket, TCS_SD_BOTH) != TCS_SUCCESS) {
            B_ERROR("Server socket failed to shutdown");
            return EC_TCS_SERVER_SOCKET_SHUTDOWN_FAILURE;
        }

        B_INFO("Server destroying server socket");
        if (tcs_destroy(&server_socket) != TCS_SUCCESS){
            B_ERROR("Server failed to destroy socket");
            return EC_TCS_SERVER_SOCKET_DESTRUCTION_FAILURE;
        }

        // NOTE: This must be called once for every time tcs_lib_init() was called!
        B_INFO("Client freeing tinycsocket");
        if (tcs_lib_free() != TCS_SUCCESS) {
            B_ERROR("Client failed to free tinycsocket");
            return EC_TCS_CLIENT_SOCKET_FREE_TINYCSOCKET_FAILURE;
        }
    }
    
    if (is_client(*application_settings)) {
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
        
        // NOTE: This must be called once for every time tcs_lib_init() was called!
        B_INFO("Client freeing tinycsocket");
        if (tcs_lib_free() != TCS_SUCCESS) {
            B_ERROR("Client failed to free tinycsocket");
            return EC_TCS_CLIENT_SOCKET_FREE_TINYCSOCKET_FAILURE;
        }
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

bool should_have_window(const ApplicationSettings settings) {
    return (settings.application_mode == AM_CLIENT) || (settings.application_mode == AM_DUAL);
}
