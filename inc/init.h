#ifndef __INIT_H
#define __INIT_H

#include <stdbool.h>
#include <sqlite3.h>
#include <safe_lib.h>
#include "cwalk.h"
#include "tinycsocket.h"
#include "debug.h"
#include "types.h"
#include "config.h"
#include "string.h"
#include "error_codes.h"
#include "networking.h"

// Fonts
#define NUM_FONTS (4)
#define PRIMARY_FONT_INDEX (0)
#define SECONDARY_FONT_INDEX (1)
#define TERTIARY_FONT_INDEX (2)
#define QUATRENARY_FONT_INDEX (3)

#define FONT_SIZE (24)

#define FONT_DAYDREAM_NUM_CHARS (259)

ErrorCode init(ApplicationSettings*, Server*, Client*, sqlite3**);

ErrorCode init_window(ApplicationSettings*);

ErrorCode init_networking(ApplicationSettings*, Server*, Client*, sqlite3**);
ErrorCode init_server(Server*, sqlite3**);
ErrorCode init_networking_server(Server*);
ErrorCode init_database(sqlite3**);
ErrorCode init_client(Client*);
ErrorCode init_networking_client(Client*);

ErrorCode uninit(ApplicationSettings*, sqlite3**);
ErrorCode uninit_networking(ApplicationSettings*, sqlite3**);
ErrorCode uninit_database(sqlite3**);

ErrorCode apply_default_application_config(ApplicationSettings*);
ErrorCode apply_default_client_config(ClientSettings*);
ErrorCode apply_default_server_config(ServerSettings*);
bool should_have_window(const ApplicationSettings);

#endif