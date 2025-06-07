#ifndef __INIT_H
#define __INIT_H

#include "types.h"
#include "debug.h"

ErrorCode init(ApplicationSettings*);
ErrorCode uninit(void);
WindowSettings populate_window_application_settings(void);

#endif