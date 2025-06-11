#ifndef __INIT_H
#define __INIT_H

#include "stdbool.h"
#include "debug.h"
#include "types.h"
#include "config.h"
#include "string.h"
#include "raylib.h"
#include "tinycsocket.h"
#include "error_codes.h"
#include "networking.h"

ErrorCode init(ApplicationSettings* settings, Server* server, Client* client);
ErrorCode init_window(ApplicationSettings* settings);
ErrorCode init_networking(ApplicationSettings* settings, Server* server, Client* client);
ErrorCode init_networking_server(Server* server);
ErrorCode init_networking_client(Client* client);

ErrorCode uninit(ApplicationSettings* settings);
ErrorCode uninit_networking(ApplicationSettings* settings);

ErrorCode apply_default_application_config(ApplicationSettings* settings);
bool should_have_window(const ApplicationSettings settings);
ErrorCode apply_default_client_config(ClientSettings* settings);
ErrorCode apply_default_server_config(ServerSettings* settings);

#endif