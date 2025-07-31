#include "init.h"

ErrorCode init(ApplicationSettings* application_settings, Server* server, Client* client, sqlite3** database) {

    B_INFO("Initializing the application");

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
    ErrorCode ec_init_window = init_window(application_settings);
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

ErrorCode init_window(ApplicationSettings* application_settings) {

    B_INFO("Initializing the window");

    return EC_OK;
}

ErrorCode init_networking(ApplicationSettings* settings, Server* server, Client* client, sqlite3** database) {

    B_INFO("Initializing networking");

    if (settings == NULL) { B_ERROR("Passed null parameter 'settings'"); return EC_PASSED_NULL; }

    // An application may be either a server, client, or both!
    static bool server_initialized = false;
    static bool client_initialized = false;

    // If this application is a server, then initialize all server related functionality
    if (is_server(*settings)) {
        ErrorCode ec_init_server = init_server(server, database);
        if (ec_init_server) {
            B_ERROR("Failed to initialize server");
            return ec_init_server;
        }

        server_initialized = true;
    }

    // If this application is a client, then initialize all client related functionality
    if (is_client(*settings)) {
        ErrorCode ec_init_client = init_client(client);
        if (ec_init_client) {
            B_ERROR("Failed to initialize client");
            return ec_init_client;
        }

        client_initialized = true;
    }

    // If neither the server nor the client were initialized, then something went wrong!
    if (!(client_initialized | server_initialized)) {
        B_ERROR("Neither the client nor server were initialized");
        return EC_NETWORKING_INIT_NO_SERVER_NOR_CLIENT_FAILURE;
    }

    return EC_OK;
}

ErrorCode init_server(Server* server, sqlite3** database) {

    B_INFO("Initializing server");

    ErrorCode ec_init_networking_server = init_networking_server(server);
    if (ec_init_networking_server) {
        B_ERROR("Failed to initialize networking server");
        return ec_init_networking_server;
    }

    ErrorCode ec_init_database = init_database(database);
    if (ec_init_database) {
        B_ERROR("Failed to initialize the database");
        return ec_init_database;
    }

    return EC_OK;
}

ErrorCode init_client(Client* client) {

    B_INFO("Initializing client");

    ErrorCode ec_init_networking_client = init_networking_client(client);
    if (ec_init_networking_client) {
        B_ERROR("Failed to initialize networking client");
        return ec_init_networking_client;
    }

    return EC_OK;
}

ErrorCode init_networking_client(Client* client) {

    B_INFO("Initializing networking client");

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

    B_INFO("Initializing networking server");

    if (server == NULL) { B_ERROR("Passed null parameter 'server'"); return EC_PASSED_NULL; }

    

    return EC_OK;
}

ErrorCode init_database(sqlite3** database) {

    B_INFO("Initializing database");

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
ErrorCode uninit(ApplicationSettings* application_settings, sqlite3** database){

    B_INFO("Uninitializing the application");

    if (application_settings == NULL) { B_ERROR("Passed null parameter 'application_settings'"); return EC_PASSED_NULL; }

    ErrorCode ec_uninit_networking = uninit_networking(application_settings, database);
    if (ec_uninit_networking) {
        B_ERROR("Failed to uninitialize networking");
        return ec_uninit_networking;
    }

    return EC_OK;
}

ErrorCode uninit_networking(ApplicationSettings* application_settings, sqlite3** database) {

    B_INFO("Uninitializing network");

    if (application_settings == NULL) { B_ERROR("Passed NULL parameter 'application_settings'"); return EC_PASSED_NULL; }

    if (is_server(*application_settings)) {

        if (database == NULL) { B_ERROR("Passed NULL parameter 'database'"); return EC_PASSED_NULL; }

        B_INFO("Server closing database");
        ErrorCode ec_uninit_database = uninit_database(database);
        if (ec_uninit_database) {
            B_ERROR("Client failed to close database");
            return ec_uninit_database;
        }
    }

    if (is_client(*application_settings)) {



    }

    return EC_OK;
}

ErrorCode uninit_database(sqlite3** database) {

    B_INFO("Uninitializing database");

    if (database == NULL) { B_ERROR("Passed NULL parameter 'database'"); return EC_PASSED_NULL; }
    if (*database == NULL) { B_ERROR("Passed NULL parameter '*database'"); return EC_PASSED_NULL; }

    ErrorCode ec_sqlite3_close = sqlite3_close(*database);
    if (ec_sqlite3_close) {
        B_ERROR("Failed to close the SQLite database");
        return ec_sqlite3_close;
    }

    return EC_OK;
}

ErrorCode uninit_window() {

    B_INFO("Uninitializing window");
    

    return EC_OK;
}

// Helper functions
ErrorCode apply_default_application_config(ApplicationSettings* settings) {

    // Apply default window settings
    settings->window_settings.dim.window_width = 1000u;
    settings->window_settings.dim.window_height = 800u;
    settings->window_settings.fullscreen = false;
    memset(settings->window_settings.window_title, 0, 256);
    memcpy_s(settings->window_settings.window_title, 256, "Multiplayer Bocce Game", strlen("Multiplayer Bocce Game"));
    settings->window_settings.target_fps = 120;

    return EC_OK;
}

ErrorCode apply_default_client_config(ClientSettings* settings) {

    // Apply default client settings
    settings->major_version = 0u;
    settings->minor_version = 0u;
    settings->patch_version = 0u;
    memset(settings->server_ip, 0, 16);
    memcpy_s(settings->server_ip, 16, "127.0.0.1", strlen("127.0.0.1"));
    settings->server_port = 57283;

    return EC_OK;
}

ErrorCode apply_default_server_config(ServerSettings* settings) {

    // Apply default server settings
    settings->major_version = 0u;
    settings->minor_version = 0u;
    settings->patch_version = 0u;
    settings->max_players = 16;

    return EC_OK;
}

bool should_have_window(const ApplicationSettings settings) {
    return (settings.application_mode == AM_CLIENT) || (settings.application_mode == AM_DUAL);
}

