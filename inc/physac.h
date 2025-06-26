#ifndef _PHYSAC_HEADER_H
#define _PHYSAC_HEADER_H

// This is a header file that simplifies the inclusion of the physac library

/* disable specific gcc warnings */
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wfloat-conversion"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wimplicit-function-declaration"
#endif

#include "raylib.h"
#include "../lib/physac/physac_real.h"

/* restore warnings */
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#endif