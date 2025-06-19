#ifndef _RAYGUI_HEADER_H
#define _RAYGUI_HEADER_H

// This is a header file that simplifies the inclusion of the Raygui library

/* disable specific gcc warnings */
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wconversion"
#endif

#include "raylib.h"
// #define RAYGUI_IMPLEMENTATION (1)
#include "../lib/raygui/raygui_real.h"
// #undef RAYGUI_IMPLEMENTATION

/* restore warnings */
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#endif