#ifndef __TYPES_H
#define __TYPES_H

#include "raylib.h"

typedef int ErrorCode;

typedef struct WindowSettings {
    int window_width;
    int window_height;
    char* window_title[256];
    int log_level;
    int target_fps;
    unsigned int config_flags;
} WindowSettings;


typedef struct ApplicationSettings {
    struct WindowSettings window_settings;
} ApplicationSettings;

#endif