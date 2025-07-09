#ifndef __NETWORKING_H
#define __NETWORKING_H

#include "tinycsocket.h"
#include "debug.h"
#include "types.h"
#include "error_codes.h"
#include "timer.h"

#define PLAYER_SOCKET_TYPE (TCS_TYPE_TCP_IP4)

typedef struct IdIndexMapEntry {
    int client_id;
    int cliend_index;
} IdIndexMapEntry;

typedef struct ServerSettings {
    int major_version;
    int minor_version;
    int patch_version;
    int port;
    int max_players;
} ServerSettings;

/// @brief Server-side data
typedef struct Server {
    ServerSettings settings;
    TcsSocket player_sockets[MAX_PLAYERS];
    // IdIndexMapEntry player_id_map[MAX_PLAYERS];
    GameState state;
} Server;

typedef struct ClientSettings {
    int major_version;
    int minor_version;
    int patch_version;
    char server_ip[16]; // "AAA.BBB.CCC.DDD\0" is 16 characters long (\0 is the null character)
    int server_port;
    int id;
} ClientSettings;

typedef struct Client {
    bool enabled;
    ClientSettings settings;
    TcsSocket socket;
    bool connected;
    GameState state;
} Client;

// Type-supporting functions
void print_window_settings(WindowSettings);
void print_client_settings(ClientSettings);
void print_server_settings(ServerSettings);
void print_application_settings(const ApplicationSettings);

bool is_server(const ApplicationSettings);
bool is_client(const ApplicationSettings);
char* get_app_type(const ApplicationSettings);

// High-level functions
ErrorCode send_data(TcsSocket socket, const uint8_t* buffer, size_t buffer_size);
ErrorCode receive_data();

// Client queries/commands
bool is_connected(const Client*);
const char* get_connected_address(const Client*);
const char* get_connected_ip(const Client*);
int get_connected_port(const Client*);

// Server queries/commands
bool is_server_full(const Server* server);
int get_num_players(const Server* server);
int get_max_num_players(const Server* server);
bool is_player_connected(const Server* server, int client_id);
ErrorCode kick_player(const Server* server, int client_id);

#endif