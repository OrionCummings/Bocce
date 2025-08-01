#include "networking.h"

// static nng_sock client_socket;
// static nng_sock server_socket;

// Server queries/commands
bool is_player_connected(const Server* server, int client_id) {
    B_ERROR("NOT IMPLEMENTED YET");
    return EC_OK;
}

ErrorCode kick_player(const Server* server, int client_id) {
    B_ERROR("NOT IMPLEMENTED YET");
    return EC_OK;
}

bool is_server_full(const Server* server){
    B_ERROR("NOT IMPLEMENTED YET");
    return false;
}

int get_num_players(const Server* server){
    B_ERROR("NOT IMPLEMENTED YET");
    return EC_OK;
}

int get_max_num_players(const Server* server){
    B_ERROR("NOT IMPLEMENTED YET");
    return EC_OK;
}

// Data transmission functions
ErrorCode send_data(int socket, const uint8_t* data, size_t data_size) {

    size_t num_bytes_sent = 0;
    // if () {
    //     B_ERROR("Client failed to send data to the server");
    //     return EC_TCS_CLIENT_SEND_FAILURE;
    // }

    // B_INFO("Sent %lld bytes to server", num_bytes_sent);

    return EC_OK;
}

ErrorCode receive_data() {
    B_ERROR("NOT IMPLEMENTED YET");
    return EC_OK;
}

// Helper functions
void print_window_settings(WindowSettings settings) {
    printf("Window Settings:\n");
    printf("\tdim: (%u,%u)\n", settings.dim.window_width, settings.dim.window_height);
    printf("\tfullscreen: '%s'\n", (settings.fullscreen) ? "true" : "false");
    printf("\twindow_title: '%s'\n", settings.window_title);
    printf("\tlog_level: '%d'\n", settings.log_level);
    printf("\ttarget_fps: '%d'\n", settings.target_fps);
    printf("\tconfig_flags: '%d'\n", settings.config_flags);
}

void print_client_settings(ClientSettings settings) {
    printf("Client Settings:\n");
    printf("\tversion: '%d.%d.%d'\n", settings.major_version, settings.minor_version, settings.patch_version);
    printf("\tserver_ip: '%s'\n", settings.server_ip);
    printf("\tserver_port: '%d'\n", settings.server_port);
}

void print_server_settings(ServerSettings settings) {
    printf("Server Settings:\n");
    printf("\tversion: '%d.%d.%d'\n", settings.major_version, settings.minor_version, settings.patch_version);
    printf("\tmax_players: '%d'\n", settings.max_players);
}

void print_application_settings(const ApplicationSettings settings) {
    print_window_settings(settings.window_settings);
}

bool is_server(const ApplicationSettings settings){
    return (settings.application_mode == AM_SERVER) || (settings.application_mode == AM_DUAL);
}

bool is_client(const ApplicationSettings settings){
    return (settings.application_mode == AM_CLIENT) || (settings.application_mode == AM_DUAL);
}

char* get_app_type(const ApplicationSettings settings) {
    bool server = is_server(settings);
    bool client = is_client(settings);
    if (server && client) {
        return "Host";
    } else if (server && !client) {
        return "Server";
    } else if (!server && client) {
        return "Client";
    } else {
        return "Unknown app type";
    }
}

