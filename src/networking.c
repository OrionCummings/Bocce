#include "networking.h"

// Server queres/commands
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
    return 0;
}

int get_max_num_players(const Server* server){
    B_ERROR("NOT IMPLEMENTED YET");
    return 0;
}

// Data transmission functions
ErrorCode send_data(TcsSocket socket, const uint8_t* buffer, size_t buffer_size) {

    size_t num_bytes_sent = 0;
    if (tcs_send(socket, buffer, buffer_size, TCS_MSG_SENDALL, &num_bytes_sent) != TCS_SUCCESS) {
        B_ERROR("Client failed to send data to the server");
        return EC_TCS_CLIENT_SEND_FAILURE;
    }

    B_INFO("Sent %lld bytes to server\n", num_bytes_sent);

    return EC_OK;
}

ErrorCode receive_data() {
    B_ERROR("NOT IMPLEMENTED YET");
    return EC_OK;
}

// TODO: Are these functions good ideas?
char* network_command_to_string(const NetworkCommand nc) {
    B_ERROR("NOT IMPLEMENTED YET");
    return NULL;
}

uint64_t network_command_to_qword(const NetworkCommand nc) {
    const uint8_t len = 64;
    uint64_t qword = 0;

    qword |= (nc.type << (len - 4));

    return qword;
}

NetworkCommand qword_to_network_command(const uint64_t qword) {
    NetworkCommand nc = {0};

    NetworkCommandType nct = ((qword & 0xF000000000000000) >> 28);
    nc.type = nct;

    return nc;
}


