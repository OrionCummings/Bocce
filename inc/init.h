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

ErrorCode init(ApplicationSettings* settings);
ErrorCode init_client(ApplicationSettings* settings);
ErrorCode init_server(ApplicationSettings* settings);
ErrorCode init_window(ApplicationSettings* settings);
ErrorCode init_networking(ApplicationSettings* settings);
ErrorCode init_networking_client(ClientSettings settings);
ErrorCode init_networking_server(ServerSettings settings);

ErrorCode uninit(ApplicationSettings* settings);
ErrorCode uninit_networking(ApplicationSettings* settings);

ErrorCode apply_default_application_config(ApplicationSettings* settings);
bool should_have_window(const ApplicationSettings settings);

#endif