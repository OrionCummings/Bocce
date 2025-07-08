#ifndef __INIT_H
#define __INIT_H

#include <stdbool.h>
#include <sqlite3.h>
#include "cwalk.h"
#include "raylib.h"
#include "tinycsocket.h"
#include "debug.h"
#include "types.h"
#include "config.h"
#include "string.h"
#include "error_codes.h"
#include "networking.h"
#include "physac.h"
#include "clay.h"
#include "clay_renderer_raylib.h"

// Fonts
#define NUM_FONTS (4)
#define PRIMARY_FONT_INDEX (0)
#define SECONDARY_FONT_INDEX (1)
#define TERTIARY_FONT_INDEX (2)
#define QUATRENARY_FONT_INDEX (3)

#define FONT_SIZE (24)

#define FONT_DAYDREAM_NUM_CHARS (259)

ErrorCode init(ApplicationSettings*, Server*, Client*, sqlite3**, Font*, Clay_Context**);

ErrorCode init_window(ApplicationSettings*, Font*, Clay_Context**);
ErrorCode init_fonts(Font* fonts);

ErrorCode init_networking(ApplicationSettings*, Server*, Client*, sqlite3**);
ErrorCode init_server(Server*, sqlite3**);
ErrorCode init_networking_server(Server*);
ErrorCode init_database(sqlite3**);
ErrorCode init_client(Client*);
ErrorCode init_networking_client(Client*);

ErrorCode uninit(ApplicationSettings*, sqlite3**, Font*);
ErrorCode uninit_networking(ApplicationSettings*, sqlite3**);
ErrorCode uninit_database(sqlite3**);
ErrorCode uninit_window(Font*);

ErrorCode apply_default_application_config(ApplicationSettings*);
ErrorCode apply_default_client_config(ClientSettings*);
ErrorCode apply_default_server_config(ServerSettings*);
bool should_have_window(const ApplicationSettings);

void clay_handle_errors(Clay_ErrorData error_data);
Clay_Dimensions clay_raylib_measure_text(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData);

#endif