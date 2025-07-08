// Some code in here is from:
// https://gist.github.com/nickolayl/f6651e978db145141faa1d986182014f

#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "debug.h"
#include "types.h"

#define ONE_MS          ((uint64_t)(       1000))
#define TEN_MS          ((uint64_t)(      10000))
#define FIFTY_MS        ((uint64_t)(      50000))
#define ONE_HUNDRED_MS  ((uint64_t)(     100000))
#define TWO_HUNDRED_MS  ((uint64_t)(     200000))
#define FIVE_HUNDRED_MS ((uint64_t)(     500000))
#define ONE_SECOND      ((uint64_t)( 1000000000))
#define TWO_SECONDS     ((uint64_t)( 2000000000))
#define FIVE_SECONDS    ((uint64_t)( 5000000000))
#define TEN_SECONDS     ((uint64_t)(10000000000))

#if defined __APPLE__
#include <mach/mach_time.h>
typedef mach_timebase_info_data_t std_Time;

#elif defined _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER
#include <windows.h>
typedef LARGE_INTEGER std_Time;

#else // posix
#ifdef _POSIX_C_SOURCE
#define NS_OLD_POSIX_C_SOURCE _POSIX_C_SOURCE
#undef _POSIX_C_SOURCE
#endif

#define _POSIX_C_SOURCE 199309L

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreserved-macro-identifier"
#ifndef __USE_POSIX199309
#define __USE_POSIX199309
#define NS_DEFINED__USE_POSIX199309
#endif

#include <time.h>
#ifdef NS_DEFINED__USE_POSIX199309
#undef __USE_POSIX199309
#endif
#pragma GCC diagnostic pop // "-Wreserved-macro-identifier"

#ifdef NS_OLD_POSIX_C_SOURCE
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE NS_OLD_POSIX_C_SOURCE
#endif
#ifdef CLOCK_MONOTONIC
#define NS_CLOCK_ID CLOCK_MONOTONIC
#else
#define NS_CLOCK_ID CLOCK_REALTIME
#endif
typedef struct timespec std_Time;
#endif

/// Monotonic Clock Data
typedef struct TimeData {
    std_Time data;
    uint64_t last;
    uint64_t current;
} TimeData;

TimeData time_start();
uint64_t time_nsec(TimeData* const clock);

typedef struct Timer {
    uint64_t start;
    uint64_t duration;
    bool expired;
} Timer;

uint64_t now(void);

Timer new_timer(uint64_t);
Timer new_timer_start(uint64_t);

void wait(Timer);

bool update_timer(Timer);

#endif