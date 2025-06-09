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

ErrorCode init(ApplicationSettings*);
ErrorCode init_window(ApplicationSettings*);
ErrorCode init_networking(ApplicationSettings*);
ErrorCode init_networking_client(ClientSettings);
ErrorCode init_networking_server(ServerSettings);

ErrorCode uninit(void);
ErrorCode uninit_networking(void);

ErrorCode apply_default_application_config(ApplicationSettings*);

#endif