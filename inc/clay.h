#ifndef _CLAY_HEADER_H
#define _CLAY_HEADER_H

// This is a header file that simplifies the inclusion of the Clay library

/* disable specific gcc warnings */
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wfloat-conversion"
#pragma GCC diagnostic ignored "-Wconversion"
#endif

#include "../lib/clay/clay_real.h"

/* restore warnings */
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#endif