#include "init.h"

// TODO: Remove
#include <unistd.h>

static TcsSocket client_socket;
static TcsSocket server_socket;
static TcsSocket listen_socket;

ErrorCode init(ApplicationSettings* application_settings, Server* server, Client* client, sqlite3** database, Font* font) {

    // Set all memory to zero
    memset(application_settings, 0, sizeof(*application_settings));
    memset(client, 0, sizeof(*client));
    memset(server, 0, sizeof(*server));

    // Apply the default config and then parse the config.toml
    // TODO: This may not be good as it populates the ApplicationSettings regardless of the ApplicationMode
    ErrorCode ec_apply_default_application_config = apply_default_application_config(application_settings);
    if (ec_apply_default_application_config) {
        B_ERROR("Failed to apply default application config");
        return ec_apply_default_application_config;
    }

    // Parse the config file
    ErrorCode ec_parse_config = parse_config(application_settings, server, client);
    if (ec_parse_config) {
        B_ERROR("Failed to parse application config");
        return ec_parse_config;
    }

    // Initialize networking
    ErrorCode ec_init_networking = init_networking(application_settings, server, client, database);
    if (ec_init_networking) {
        B_ERROR("Failed to initialize networking");
        return ec_init_networking;
    }

    ErrorCode ec_init_window = init_window(application_settings, font);
    if (ec_init_window) {
        B_ERROR("Failed to initialize the window");
        return ec_init_window;
    }

    // TODO: Initialize arena allocator
    // TODO: Initialize audio system
    // TODO: Initialize obj-file parser
    // TODO: Initialize database

    return EC_OK;
}

ErrorCode init_window(ApplicationSettings* application_settings, Font* font) {

    // Initialize and configure the window
    B_INFO("Initializing Raylib 5.6 and configuring the main window");
    WindowSettings ws = application_settings->window_settings;
    SetConfigFlags(ws.config_flags); // FLAG_MSAA_4X_HINT
    SetTraceLogLevel(ws.log_level);
    SetTargetFPS(ws.target_fps);
    InitWindow(ws.window_width, ws.window_height, ws.window_title);
    HideCursor();

    // Load font(s)
    // TODO: Make this actually handle multiple fonts lol
    init_fonts(font);

    // Init physics
    InitPhysics();

    SetTextLineSpacing(16);

    return EC_OK;
}

ErrorCode init_fonts(Font* font) {

    if (font == NULL) { B_ERROR("Passed null parameter 'font'"); return EC_PASSED_NULL; }

    char cwd[_MAX_PATH] = { 0 };
    getcwd(cwd, _MAX_PATH);
    printf("cwd = %s\n", cwd);
    
    /*

    struct cwk_segment segment;

    if (!cwk_path_get_last_segment(cwd, &segment)) {
        B_ERROR("CWD has no segments! Are you working straight from the C: drive or home directory?");
        return EC_CWD_NO_SEGMENTS;
    }

    do {
        // printf("Current segment is '%.*s'.\n", (int)segment.size, segment.begin);
        printf("Current segment is '%s'.\n", segment.begin);

        char segment_name[segment.size];
        strncpy_s(segment_name, segment.size, segment.begin + (segment.size - strnlen_s(segment.begin, segment.size * sizeof(*segment.begin))), segment.size);


        // if (!strcmp(segment.begin, "Bocce")) {
        //     break;
        // }
        printf("%s\n", segment_name);
    } while (cwk_path_get_previous_segment(&segment));
    
    printf("Last segment is '%.*s'.\n", (int)segment.size, segment.begin);
    */
   
   // TODO: Make the font selection better
    // Make sure the font file exists
    const char* font_path_release = "res\\fonts\\daydream\\daydream.ttf";
    const char* font_path_debug = "..\\..\\..\\res\\fonts\\daydream\\daydream.ttf";
    if (access(font_path_release, F_OK) == 0) {
        B_INFO("Loaded font 'daydream'");
        *font = LoadFontEx(font_path_release, FONT_SIZE, NULL, FONT_DAYDREAM_NUM_CHARS);
    } else if (access(font_path_debug, F_OK) == 0) {
        B_INFO("Loaded font 'daydream'");
        *font = LoadFontEx(font_path_debug, FONT_SIZE, NULL, FONT_DAYDREAM_NUM_CHARS);
    } else {
        B_INFO("Failed to load font 'daydream'; falling back to default font");
        *font = GetFontDefault();
    }

    ErrorCode ec_verify_fonts = verify_font(*font);
    if (!ec_verify_fonts) {
        B_WARNING("Some fonts failed to load");
    }

    return EC_OK;
}

ErrorCode verify_fonts(const Font* fonts) {

    if (fonts == NULL) { B_ERROR("Passed null parameter 'fonts'"); return EC_PASSED_NULL; }

    bool invalid_fonts = false;
    for (size_t index = 0; index < NUM_FONTS; index++) {
        if (!verify_font(fonts[index])) {
            B_WARNING("Failed to verify font '%d'", index);
            invalid_fonts = true;
        }
    }

    return (ErrorCode)invalid_fonts;
}

ErrorCode verify_font(const Font font) {
    return (ErrorCode)(font.glyphCount > 0);
}

ErrorCode init_networking(ApplicationSettings* settings, Server* server, Client* client, sqlite3** database) {

    if (settings == NULL) { B_ERROR("Passed null parameter 'settings'"); return EC_PASSED_NULL; }

    if (is_server(*settings)) {
        B_INFO("Initializing the networking server");
        ErrorCode ec_init_networking_server = init_networking_server(server);
        if (ec_init_networking_server) {
            B_ERROR("Failed to initialize networking server");
            return ec_init_networking_server;
        }

        B_INFO("Initializing the SQLite database");
        ErrorCode ec_init_database = init_database(database);
        if (ec_init_database) {
            B_ERROR("Failed to initialize the database");
            return ec_init_database;
        }
    }

    if (is_client(*settings)) {
        ErrorCode ec_init_networking_client = init_networking_client(client);
        if (ec_init_networking_client) {
            B_ERROR("Failed to initialize networking client");
            return ec_init_networking_client;
        }
        B_INFO("Initialized the networking client");
    }

    return EC_OK;
}

ErrorCode init_networking_client(Client* client) {

    if (client == NULL) {
        B_ERROR("Passed null parameter 'client'");
        return EC_PASSED_NULL;
    }

    if (tcs_lib_init() != TCS_SUCCESS) {
        B_ERROR("Client failed to initialize tinycsockets");
        return EC_TCS_CLIENT_INIT_FAILURE;
    }

    client_socket = TCS_NULLSOCKET;
    if (tcs_create(&client_socket, TCS_TYPE_TCP_IP4) != TCS_SUCCESS) {
        B_ERROR("Client failed to create socket");
        return EC_TCS_CLIENT_SOCKET_CREATE_FAILURE;
    }

    const char* ip = client->settings.server_ip;
    const uint16_t port = (uint16_t)(client->settings.server_port);

    if (tcs_connect(client_socket, ip, port) != TCS_SUCCESS) {
        B_ERROR("Client failed to connect to server");
        return EC_TCS_CLIENT_CONNECTION_FAILURE;
    }

    size_t n = 32; // TODO: Magic number!
    char buffer[n];
    memset(buffer, 0, n * sizeof(buffer[0]));
    const char* connection_new_fmt_string = "CN %d";
    snprintf(buffer, n, connection_new_fmt_string, client->settings.id);

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

ErrorCode init_networking_server(Server* server) {

    if (server == NULL) { B_ERROR("Passed null parameter 'server'"); return EC_PASSED_NULL; }

    B_ERROR("Server initializing tinycsockets");
    if (tcs_lib_init() != TCS_SUCCESS) {
        B_ERROR("Server failed to initialize tinycsockets");
        return EC_TCS_SERVER_INIT_FAILURE;
    }

    B_ERROR("Server creating listening socket");
    server_socket = TCS_NULLSOCKET;
    listen_socket = TCS_NULLSOCKET;
    if (tcs_create(&listen_socket, TCS_TYPE_TCP_IP4) != TCS_SUCCESS) {
        B_ERROR("Server failed to create listening socket");
        return EC_TCS_LISTEN_SOCKET_CREATE_FAILURE;
    }

    uint16_t listen_port = (uint16_t)(server->settings.port);
    B_ERROR("Server listening on port '%d'", listen_port);
    if (tcs_listen_to(listen_socket, listen_port)) {
        B_ERROR("Server failed to listen on port '%d'", listen_port);
        return EC_TCS_LISTEN_SOCKET_LISTEN_FAILURE;
    }

    B_ERROR("Server attempting to accept connection");
    if (tcs_accept(listen_socket, &server_socket, NULL)) {
        B_ERROR("Server failed to accept connection");
        return EC_TCS_LISTEN_SOCKET_CONNCTION_ACCEPTANCE_FAILURE;
    }

    B_ERROR("Server destroying listening socket");
    if (tcs_destroy(&listen_socket) != TCS_SUCCESS) {
        B_ERROR("Server failed to destroy listening socket");
        return EC_TCS_LISTEN_SOCKET_DESTRUCTION_FAILURE;
    }

    uint8_t recv_buffer[1024]; // TODO: Remove magic numbers!
    size_t recv_size = sizeof(recv_buffer) - sizeof('\0');
    size_t bytes_received = 0;
    B_ERROR("Server attempting to receive data from client");
    if (tcs_receive(server_socket, recv_buffer, recv_size, TCS_NO_FLAGS, &bytes_received) != TCS_SUCCESS) {
        B_ERROR("Server failed to receive data from client");
        return EC_TCS_SERVER_SOCKET_RECEPTION_FAILURE;
    }

    recv_buffer[bytes_received] = '\0';
    B_INFO("received: '%s'", recv_buffer);

    char msg[] = "CN.ACK\n";
    B_ERROR("Server attempting to send data to client");
    if (tcs_send(server_socket, (const uint8_t*)msg, sizeof(msg), TCS_MSG_SENDALL, NULL) != TCS_SUCCESS) {
        B_ERROR("Server failed to send data to client");
        return EC_TCS_SERVER_DATA_TRANSMISSION_FAILURE;
    }

    return EC_OK;
}

ErrorCode init_database(sqlite3** database) {

    if (database == NULL) { B_ERROR("Passed null parameter 'database'"); return EC_PASSED_NULL; }

    B_INFO("Initializing the database");
    const char* database_path = "db/server.db";
    char* error_message = 0;
    ErrorCode ec_sqlite3_open = sqlite3_open(database_path, database);
    if (ec_sqlite3_open || database == NULL) {
        B_ERROR("Failed to open the SQLite database");
        return ec_sqlite3_open;
    }

    return EC_OK;
}

// Uninitialzation functions
ErrorCode uninit(ApplicationSettings* application_settings, sqlite3** database, Font* font){

    if (application_settings == NULL) { B_ERROR("Passed null parameter 'application_settings'"); return EC_PASSED_NULL; }

    B_INFO("Uninitializing network");
    ErrorCode ec_uninit_networking = uninit_networking(application_settings, database);
    if (ec_uninit_networking) {
        B_ERROR("Failed to uninitialize networking");
        return ec_uninit_networking;
    }

    B_INFO("Uninitializing the window");
    ErrorCode ec_uninit_window = uninit_window(font);
    if (ec_uninit_window) {
        B_ERROR("Failed to uninitialize the window");
        return ec_uninit_window;
    }

    return EC_OK;
}

ErrorCode uninit_networking(ApplicationSettings* application_settings, sqlite3** database) {

    if (application_settings == NULL) { B_ERROR("Passed NULL parameter 'application_settings'"); return EC_PASSED_NULL; }

    if (is_server(*application_settings)) {

        if (database == NULL) { B_ERROR("Passed NULL parameter 'database'"); return EC_PASSED_NULL; }

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

        B_INFO("Server freeing tinycsocket");
        if (tcs_lib_free() != TCS_SUCCESS) {
            B_ERROR("Client failed to free tinycsocket");
            return EC_TCS_CLIENT_SOCKET_FREE_TINYCSOCKET_FAILURE;
        }

        B_INFO("Server closing database");
        ErrorCode ec_uninit_database = uninit_database(database);
        if (ec_uninit_database) {
            B_ERROR("Client failed to close database");
            return ec_uninit_database;
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

        B_INFO("Client freeing tinycsocket");
        if (tcs_lib_free() != TCS_SUCCESS) {
            B_ERROR("Client failed to free tinycsocket");
            return EC_TCS_CLIENT_SOCKET_FREE_TINYCSOCKET_FAILURE;
        }
    }

    return EC_OK;
}

ErrorCode uninit_database(sqlite3** database) {

    if (database == NULL) { B_ERROR("Passed NULL parameter 'database'"); return EC_PASSED_NULL; }
    if (*database == NULL) { B_ERROR("Passed NULL parameter '*database'"); return EC_PASSED_NULL; }

    ErrorCode ec_sqlite3_close = sqlite3_close(*database);
    if (ec_sqlite3_close) {
        B_ERROR("Failed to close the SQLite database");
        return ec_sqlite3_close;
    }

    return EC_OK;
}

ErrorCode uninit_window(Font* font) {

    // Unload the fonts
    B_INFO("Unloading font(s)");
    UnloadFont(*font);
    
    // Unload the physics engine
    B_INFO("Unloading physics");
    ClosePhysics();

    // Close the window
    B_INFO("Unloading the window");
    CloseWindow();

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

    return EC_OK;
}

ErrorCode apply_default_client_config(ClientSettings* settings) {

    // Apply default client settings
    settings->major_version = -1;
    settings->minor_version = -1;
    settings->patch_version = -1;
    memset(settings->server_ip, 0, 16);
    memcpy_s(settings->server_ip, 16, "127.0.0.1", strlen("127.0.0.1"));
    settings->server_port = 57283;

    return EC_OK;
}

ErrorCode apply_default_server_config(ServerSettings* settings) {

    // Apply default server settings
    settings->major_version = -1;
    settings->minor_version = -1;
    settings->patch_version = -1;
    settings->max_players = 16;

    return EC_OK;
}

bool should_have_window(const ApplicationSettings settings) {
    return (settings.application_mode == AM_CLIENT) || (settings.application_mode == AM_DUAL);
}
