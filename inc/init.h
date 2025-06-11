#ifndef __INIT_H
#define __INIT_H

#include <stdbool.h>
#include <sqlite3.h>
#include "debug.h"
#include "types.h"
#include "config.h"
#include "string.h"
#include "raylib.h"
#include "tinycsocket.h"
#include "error_codes.h"
#include "networking.h"

ErrorCode init(ApplicationSettings*, Server*, Client*, sqlite3**);
ErrorCode init_window(ApplicationSettings*);
ErrorCode init_networking(ApplicationSettings*, Server*, Client*, sqlite3**);
ErrorCode init_networking_server(Server*);
ErrorCode init_networking_client(Client*);
ErrorCode init_database(sqlite3**);

ErrorCode uninit(ApplicationSettings*, sqlite3**);
ErrorCode uninit_networking(ApplicationSettings*, sqlite3**);
ErrorCode uninit_database(sqlite3**);

ErrorCode apply_default_application_config(ApplicationSettings*);
ErrorCode apply_default_client_config(ClientSettings*);
ErrorCode apply_default_server_config(ServerSettings*);
bool should_have_window(const ApplicationSettings);

#endif