#ifndef __NETWORKING_H
#define __NETWORKING_H

#include "tinycsocket.h"
#include "debug.h"
#include "types.h"
#include "error_codes.h"

#define MAX_PLAYERS (16) // NOTE: This is independent of the config.toml

typedef struct IdIndexMapEntry {
    int client_id;
    int cliend_index;
} IdIndexMapEntry;

typedef struct Server {
    IdIndexMapEntry player_id_map[MAX_PLAYERS]; //

} Server;

// 16 types
typedef enum NetworkCommandType {
    NCT_UNKNOWN = 0,
    NCT_CONNECT = 1,
    NCT_CONNECT_ACK = 2,
    NCT_DISCONNECT = 3,
    NCT_DISCONNECT_ACK = 4,
    NCT_5 = 5,
    NCT_6 = 6,
    NCT_7 = 7,
    NCT_8 = 8,
    NCT_9 = 9,
    NCT_10 = 10,
    NCT_11 = 11,
    NCT_12 = 12,
    NCT_13 = 13,
    NCT_14 = 14,
    NCT_15 = 15,
} NetworkCommandType;

typedef struct NetworkCommand {
    NetworkCommandType type;
} NetworkCommand;

// High-level functions
ErrorCode send_data(TcsSocket socket, const uint8_t* buffer, size_t buffer_size);
ErrorCode receive_data();

// Server queries/commands
bool is_server_full(const Server* server);
int get_num_players(const Server* server);
int get_max_num_players(const Server* server);
bool is_player_connected(const Server* server, int client_id);
ErrorCode kick_player(const Server* server, int client_id);

// TODO: maybe remove?
char* network_command_to_string(const NetworkCommand);
uint64_t network_command_to_qword(const NetworkCommand);
NetworkCommand qword_to_network_command(const uint64_t);




#endif