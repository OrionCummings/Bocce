#ifndef __INIT_H
#define __INIT_H

#include "types.h"
#include "debug.h"
#include "config.h"
#include "raylib.h"
#include "stdbool.h"

ErrorCode init(ApplicationSettings*);
ErrorCode uninit(void);

ErrorCode apply_default_config(ApplicationSettings*);

#endif