#ifndef __TYPES_H
#define __TYPES_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

typedef int ErrorCode;

typedef struct WindowSettings {
    int window_width;
    int window_height;
    bool fullscreen;
    char window_title[256];
    int log_level;
    int target_fps;
    unsigned int config_flags;
} WindowSettings;

typedef struct ClientSettings {
    int major_version;
    int minor_version;
    int patch_version;
    char server_ip[16]; // "AAA.BBB.CCC.DDD\0" is 16 characters long (\0 is the null character)
    int server_port;
} ClientSettings;

typedef struct ServerSettings {
    int major_version;
    int minor_version;
    int patch_version;
    int max_players;
} ServerSettings;

typedef struct ApplicationSettings {
    WindowSettings window_settings;
    ClientSettings client_settings;
    ServerSettings server_settings;
} ApplicationSettings;

// Type-supporting functions
void print_window_settings(WindowSettings);
void print_client_settings(ClientSettings);
void print_server_settings(ServerSettings);
void print_application_settings(const ApplicationSettings);

#endif