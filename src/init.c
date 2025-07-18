#include "init.h"

// TODO: Remove
#include <unistd.h>

static TcsSocket client_sockets[MAX_CONNECTIONS];
static TcsSocket listen_socket;
static TcsSocket client_socket;

static Clay_Arena clay_memory;

ErrorCode init(ApplicationSettings* application_settings, Server* server, Client* client, sqlite3** database, Font* fonts, Clay_Context** context) {

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

    // Initialize window
    ErrorCode ec_init_window = init_window(application_settings, fonts, context);
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

ErrorCode init_window(ApplicationSettings* application_settings, Font* fonts, Clay_Context** context) {

    // Initialize and configure the window
    B_INFO("Initializing Raylib 5.6 and configuring the main window");

    WindowSettings ws = application_settings->window_settings;

    size_t clay_required_memory = Clay_MinMemorySize();
    clay_memory = Clay_CreateArenaWithCapacityAndMemory(clay_required_memory, malloc(clay_required_memory));
    *context = Clay_Initialize(
        clay_memory, (Clay_Dimensions) {
        .width = (float)ws.window_width,
            .height = (float)ws.window_height
    },
        (Clay_ErrorHandler) {
        .errorHandlerFunction = clay_handle_errors,
            .userData = NULL
    }
    );
    SetConfigFlags(ws.config_flags); // FLAG_MSAA_4X_HINT
    SetTraceLogLevel(ws.log_level);
    SetTargetFPS(ws.target_fps);
    Clay_Raylib_Initialize(ws.window_width, ws.window_height, ws.window_title, FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);

    // TODO: Refactor this into init_fonts()
    // TODO: Make this actually handle multiple fonts lol
    init_fonts(fonts);
    SetTextureFilter(fonts[0].texture, TEXTURE_FILTER_BILINEAR);
    Clay_SetMeasureTextFunction(clay_raylib_measure_text, fonts);

    // Init physics
    InitPhysics();

    SetTextLineSpacing(16);

    return EC_OK;
}

ErrorCode init_fonts(Font* fonts) {

    if (fonts == NULL) { B_ERROR("Passed null parameter 'font'"); return EC_PASSED_NULL; }

    // char cwd[_MAX_PATH] = { 0 };
    // getcwd(cwd, _MAX_PATH);
    // B_INFO("cwd = %s\n", cwd);

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
    const char* font_path_release = "res/fonts/daydream/daydream.ttf";
    const char* font_path_debug = "../../../res/fonts/daydream/daydream.ttf";
    if (access(font_path_release, F_OK) == 0) {
        B_INFO("Loaded font 'daydream' from the release path");
        fonts[0] = LoadFontEx(font_path_release, 128, NULL, FONT_DAYDREAM_NUM_CHARS);
    } else if (access(font_path_debug, F_OK) == 0) {
        B_INFO("Loaded font 'daydream' from the debug path");
        fonts[0] = LoadFontEx(font_path_debug, 128, NULL, FONT_DAYDREAM_NUM_CHARS);
    } else {
        B_INFO("Failed to load font 'daydream'; falling back to default font");
        fonts[0] = GetFontDefault();
    }

    return EC_OK;
}

ErrorCode init_networking(ApplicationSettings* settings, Server* server, Client* client, sqlite3** database) {

    if (settings == NULL) { B_ERROR("Passed null parameter 'settings'"); return EC_PASSED_NULL; }

    // An application may be either a server, client, or both!
    static bool server_initialized = false;
    static bool client_initialized = false;

    B_INFO("Initializing tinycsockets");
    if (tcs_lib_init() != TCS_SUCCESS) {
        B_ERROR("Failed to initialize tinycsockets");
        return EC_TCS_SERVER_INIT_FAILURE;
    }

    // If this application is a server, then initialize all server related functionality
    if (is_server(*settings)) {
        ErrorCode ec_init_server = init_server(server, database);
        if (ec_init_server) {
            B_ERROR("Failed to initialize server");
            return ec_init_server;
        }

        server_initialized = true;
        B_INFO("Initialized the networking server");
    }

    // If this application is a client, then initialize all client related functionality
    if (is_client(*settings)) {
        ErrorCode ec_init_client = init_client(client);
        if (ec_init_client) {
            B_ERROR("Failed to initialize client");
            return ec_init_client;
        }

        client_initialized = true;
        B_INFO("Initialized the networking client");
    }

    // If neither the server nor the client were initialized, then something went wrong!
    if (!(client_initialized | server_initialized)) {
        B_ERROR("Neither the client nor server were initialized");
        return EC_NETWORKING_INIT_NO_SERVER_NOR_CLIENT_FAILURE;
    }

    return EC_OK;
}

ErrorCode init_server(Server* server, sqlite3** database) {
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

    return EC_OK;
}

ErrorCode init_client(Client* client) {
    ErrorCode ec_init_networking_client = init_networking_client(client);
    if (ec_init_networking_client) {
        B_ERROR("Failed to initialize networking client");
        return ec_init_networking_client;
    }

    return EC_OK;
}

ErrorCode init_networking_client(Client* client) {

    if (client == NULL) {
        B_ERROR("Passed null parameter 'client'");
        return EC_PASSED_NULL;
    }

    if (!client->enabled) {
        B_WARNING("Client networking disabled; not initializing!");
        return EC_OK;
    }

    // client_socket = TCS_NULLSOCKET;
    // B_INFO("Client creating socket");
    // if (tcs_create(&client_socket, TCS_TYPE_TCP_IP4) != TCS_SUCCESS) {
    //     B_ERROR("Client failed to create socket");
    //     return EC_TCS_CLIENT_SOCKET_CREATE_FAILURE;
    // }

    // const char* ip = client->settings.server_ip;
    // const uint16_t port = (uint16_t)(client->settings.server_port);

    // if (tcs_connect(client_socket, ip, port) != TCS_SUCCESS) {
    //     B_ERROR("Client failed to connect to server");
    //     B_WARNING("Ignoring server connection failure");
    //     // return EC_TCS_CLIENT_CONNECTION_FAILURE;
    //     return EC_OK;
    // }

    // size_t n = 32; // TODO: Magic number!
    // char buffer[n];
    // memset(buffer, 0, n * sizeof(buffer[0]));
    // const char* connection_new_fmt_string = "CN %d";
    // snprintf(buffer, n, connection_new_fmt_string, client->settings.id);

    // size_t size = sizeof(buffer);
    // const uint8_t* cbuffer = (const uint8_t*)buffer;
    // ErrorCode ec_send = send_data(client_socket, cbuffer, size);
    // if (ec_send) {
    //     // if (tcs_send(client_socket, cbuffer, size, TCS_MSG_SENDALL, NULL) != TCS_SUCCESS) {
    //     B_ERROR("Client failed to send data to the server");
    //     return ec_send;
    //     // return EC_TCS_CLIENT_SEND_FAILURE;
    // }

    // uint8_t recv_buffer[1024];
    // size_t recv_size = sizeof(recv_buffer) - sizeof('\0');
    // size_t bytes_received = 0;
    // if (tcs_receive(client_socket, recv_buffer, recv_size, TCS_NO_FLAGS, &bytes_received) != TCS_SUCCESS) {
    //     B_ERROR("Client failed to receive data from the server");
    //     return EC_TCS_CLIENT_RECEPTION_FAILURE;
    // }

    // recv_buffer[bytes_received] = '\0';
    // B_INFO("Received: '%s'", recv_buffer);

    return EC_OK;
}

/// @brief Initializes the server side networking
/// @param server The server data
/// @return An error code indicating any errors that occured in this function
ErrorCode init_networking_server(Server* server) {

    if (server == NULL) { B_ERROR("Passed null parameter 'server'"); return EC_PASSED_NULL; }

    // Create & configure the socket pool
    struct TcsPool* pool;
    for (uint8_t socket_id = 0; socket_id < MAX_CONNECTIONS; socket_id++) {

        // Create the socket the socket
        client_sockets[socket_id] = TCS_NULLSOCKET;
        TcsReturnCode rc_tcs_create = tcs_create(&(client_sockets[socket_id]), PLAYER_SOCKET_TYPE);
        if (rc_tcs_create) {
            B_ERROR("Server failed to create socket id '%d'", socket_id);
            return (ErrorCode)rc_tcs_create;
        }

        // Add the socket to the pool
        tcs_pool_add(server->socket_pool, client_sockets[socket_id], NULL, true, true, true);
    }

    uint64_t time_start = now();
    B_INFO("[%lld] Starting timer", time_start);
    Timer t = new_timer(TWO_SECONDS);
    wait(t);
    uint64_t time_end = now();
    uint64_t delta = time_end - time_start;
    B_INFO("[%lld] Ending timer", time_end);
    B_INFO("Took %llds / %lldms / %lldns", delta / ONE_SECOND, delta / ONE_MS, delta);

    // tcs_pool_create(,);

    // // listen_socket = TCS_NULLSOCKET;
    // for (uint8_t connection_id = 0; connection_id < MAX_CONNECTIONS; connection_id++) {
    //     server->player_sockets[connection_id]
    // }

    // B_INFO("Server creating listening socket");
    // server_socket = TCS_NULLSOCKET;
    // listen_socket = TCS_NULLSOCKET;
    // if (tcs_create(&listen_socket, TCS_TYPE_TCP_IP4) != TCS_SUCCESS) {
    //     B_ERROR("Server failed to create listening socket");
    //     return EC_TCS_LISTEN_SOCKET_CREATE_FAILURE;
    // }

    B_INFO("Server creating socket pool");
    // tcs_pool_create();

    return EC_OK;
}

ErrorCode init_database(sqlite3** database) {

    if (database == NULL) { B_ERROR("Passed null parameter 'database'"); return EC_PASSED_NULL; }

    B_INFO("Initializing the database");
    const char* database_path = "C:\\Users\\Orion\\Stash\\PersonalProjects\\_C\\Bocce\\db\\server.db";
    char* error_message = 0;
    ErrorCode ec_sqlite3_open = sqlite3_open(database_path, database);
    if (database == NULL) {
        B_ERROR("Failed to allocate memory for the sqlite3 object");
        return EC_INIT_DATABASE_FAILED_TO_ALLOCATE;
    }
    if (ec_sqlite3_open) {
        B_ERROR("Failed to open the SQLite database (%d)", ec_sqlite3_open);
        return ec_sqlite3_open;
    }

    return EC_OK;
}

// Uninitialzation functions
ErrorCode uninit(ApplicationSettings* application_settings, sqlite3** database, Font* fonts){

    if (application_settings == NULL) { B_ERROR("Passed null parameter 'application_settings'"); return EC_PASSED_NULL; }

    B_INFO("Uninitializing network");
    ErrorCode ec_uninit_networking = uninit_networking(application_settings, database);
    if (ec_uninit_networking) {
        B_ERROR("Failed to uninitialize networking");
        return ec_uninit_networking;
    }

    B_INFO("Uninitializing the window");
    ErrorCode ec_uninit_window = uninit_window(fonts);
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

        B_INFO("Server shutting down server sockets");
        for (uint8_t client_socket_index = 0; client_socket_index < MAX_CONNECTIONS; client_socket_index++) {
            if (tcs_shutdown(client_sockets[client_socket_index], TCS_SD_BOTH) != TCS_SUCCESS) {
                B_ERROR("Server socket '%d' failed to shutdown", client_socket_index);
                return EC_TCS_SERVER_SOCKET_SHUTDOWN_FAILURE;
            }

            B_INFO("Server destroying server sockets");
            if (tcs_destroy(&(client_sockets[client_socket_index])) != TCS_SUCCESS){
                B_ERROR("Server failed to destroy socket");
                return EC_TCS_SERVER_SOCKET_DESTRUCTION_FAILURE;
            }

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

        if (!client_socket) {
            B_WARNING("Client networking disabled; not uninitializing!");
            return EC_OK;
        }

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

    // Unload Clay
    B_INFO("Unloading Clay");
    Clay_Raylib_Close();

    // Close the window
    B_INFO("Unloading the window");
    CloseWindow(); // TODO: Figure out why it seems to crash here!

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

void clay_handle_errors(Clay_ErrorData error_data) {
    B_ERROR("%s", error_data.errorText.chars);
    if (error_data.errorType == CLAY_ERROR_TYPE_ELEMENTS_CAPACITY_EXCEEDED) {
        Clay_SetMaxElementCount(Clay_GetMaxElementCount() * 2);
    } else if (error_data.errorType == CLAY_ERROR_TYPE_TEXT_MEASUREMENT_CAPACITY_EXCEEDED) {
        Clay_SetMaxMeasureTextCacheWordCount(Clay_GetMaxMeasureTextCacheWordCount() * 2);
    }
}

Clay_Dimensions clay_raylib_measure_text(Clay_StringSlice text, Clay_TextElementConfig* config, void* user_data) {

    // Measure string size for Font
    Clay_Dimensions textSize = { 0 };

    float maxTextWidth = 0.0f;
    float lineTextWidth = 0;
    int maxLineCharCount = 0;
    int lineCharCount = 0;

    float textHeight = config->fontSize;
    Font* fonts = (Font*)user_data;
    Font fontToUse = fonts[config->fontId];
    // Font failed to load, likely the fonts are in the wrong place relative to the execution dir.
    // RayLib ships with a default font, so we can continue with that built in one. 
    if (!fontToUse.glyphs) {
        fontToUse = GetFontDefault();
    }

    float scaleFactor = config->fontSize / (float)fontToUse.baseSize;

    for (int i = 0; i < text.length; ++i, lineCharCount++)
    {
        if (text.chars[i] == '\n') {
            maxTextWidth = fmaxf(maxTextWidth, lineTextWidth);
            maxLineCharCount = CLAY__MAX(maxLineCharCount, lineCharCount);
            lineTextWidth = 0;
            lineCharCount = 0;
            continue;
        }
        int index = text.chars[i] - 32;
        if (fontToUse.glyphs[index].advanceX != 0) lineTextWidth += (float)fontToUse.glyphs[index].advanceX;
        else lineTextWidth += (fontToUse.recs[index].width + (float)fontToUse.glyphs[index].offsetX);
    }

    maxTextWidth = fmaxf(maxTextWidth, lineTextWidth);
    maxLineCharCount = CLAY__MAX(maxLineCharCount, lineCharCount);

    textSize.width = maxTextWidth * scaleFactor + (float)(lineCharCount * config->letterSpacing);
    textSize.height = textHeight;

    return textSize;
}

