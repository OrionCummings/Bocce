#include "networking.h"

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

// Server functions
ErrorCode check_for_new_connections(const Server server, TcsSocket server_socket, TcsSocket listen_socket) {

    // uint16_t listen_port = (uint16_t)(server.settings.port);
    // B_ERROR("Server listening on port '%d'", listen_port);
    // if (tcs_listen_to(listen_socket, listen_port)) {
    //     B_ERROR("Server failed to listen on port '%d'", listen_port);
    //     return EC_TCS_LISTEN_SOCKET_LISTEN_FAILURE;
    // }

    B_ERROR("Server attempting to accept connection");
    if (tcs_accept(listen_socket, &server_socket, NULL)) {
        B_ERROR("Server failed to accept connection");
        return EC_TCS_LISTEN_SOCKET_CONNCTION_ACCEPTANCE_FAILURE;
    }

    // B_ERROR("Server destroying listening socket");
    // if (tcs_destroy(&listen_socket) != TCS_SUCCESS) {
    //     B_ERROR("Server failed to destroy listening socket");
    //     return EC_TCS_LISTEN_SOCKET_DESTRUCTION_FAILURE;
    // }

    // uint8_t recv_buffer[1024]; // TODO: Remove magic numbers!
    // size_t recv_size = sizeof(recv_buffer) - sizeof('\0');
    // size_t bytes_received = 0;
    // B_ERROR("Server attempting to receive data from client");
    // if (tcs_receive(server_socket, recv_buffer, recv_size, TCS_NO_FLAGS, &bytes_received) != TCS_SUCCESS) {
    //     B_ERROR("Server failed to receive data from client");
    //     return EC_TCS_SERVER_SOCKET_RECEPTION_FAILURE;
    // }

    // recv_buffer[bytes_received] = '\0';
    // B_INFO("received: '%s'", recv_buffer);

    // char msg[] = "CN.ACK\n";
    // B_ERROR("Server attempting to send data to client");
    // if (tcs_send(server_socket, (const uint8_t*)msg, sizeof(msg), TCS_MSG_SENDALL, NULL) != TCS_SUCCESS) {
    //     B_ERROR("Server failed to send data to client");
    //     return EC_TCS_SERVER_DATA_TRANSMISSION_FAILURE;
    // }

    return EC_OK;
}

// Data transmission functions
ErrorCode send_data(TcsSocket socket, const uint8_t* buffer, size_t buffer_size) {

    size_t num_bytes_sent = 0;
    if (tcs_send(socket, buffer, buffer_size, TCS_MSG_SENDALL, &num_bytes_sent) != TCS_SUCCESS) {
        B_ERROR("Client failed to send data to the server");
        return EC_TCS_CLIENT_SEND_FAILURE;
    }

    B_INFO("Sent %lld bytes to server", num_bytes_sent);

    return EC_OK;
}

ErrorCode receive_data() {
    B_ERROR("NOT IMPLEMENTED YET");
    return EC_OK;
}

// Helper functions
void print_window_settings(WindowSettings settings) {
    printf("Window Settings:\n");
    printf("\twindow_width: '%d'\n", settings.window_width);
    printf("\twindow_height:' %d'\n", settings.window_height);
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

