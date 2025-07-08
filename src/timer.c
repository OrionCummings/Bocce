#include "timer.h"

// #if defined _WIN32
// TimeData time_start_win() {
//     TimeData result = (TimeData){ .data = (std_Time){} };
//     QueryPerformanceFrequency(&result.data);
//     return result;
// }

// #elif defined __APPLE__
// static TimeData time_start_mac() {
//     TimeData result = (TimeData){ .data = (std_Time){} };
//     mach_timebase_info(&result.data);
//     return result;
// }

// #else
// static TimeData time_start_lnx() {
//     TimeData result = (TimeData){ .data = (std_Time){} };
//     clock_getres(NS_CLOCK_ID, &result.data);
//     return result;
// }
// #endif

// TimeData time_start() {
// #if defined _WIN32
//     return time_start_win();
// #elif defined __APPLE__
//     return time_start_mac();
// #else
//     return time_start_lnx();
// #endif
// }

// #if defined _WIN32
// uint64_t time_nsec_win(TimeData* const clock) {
//     std_Time time;
//     QueryPerformanceCounter(&time);
//     clock->last = clock->current;
//     clock->current = (uint64_t)((1.0e9 * (float)time.QuadPart) / (float)clock->data.QuadPart);
//     return clock->current;
// }

// #elif defined __APPLE__
// static uint64_t time_nsec_mac(TimeData* const clock) {
//     clock->last = clock->current;
//     clock->current = mach_absolute_time();
//     clock->current *= clock->data.numer;
//     clock->current /= clock->data.denom;
//     return clock->current;
// }

// #else
// static uint64_t time_nsec_lnx(TimeData* const clock) {
//     std_Time time;
//     clock_gettime(NS_CLOCK_ID, &time);
//     clock->last = clock->current;
//     clock->current = (uint64_t)time.tv_sec * (uint64_t)1.0e9 + (uint64_t)time.tv_nsec;
//     return clock->current;
// }
// #endif

// uint64_t now() {
//     TimeData clock;
// #if defined _WIN32
//     return time_nsec_win(&clock);
// #elif defined __APPLE__
//     return time_nsec_mac(&clock);
// #else
//     return time_nsec_lnx(&clock);
// #endif
// }

uint64_t now() {
    struct timespec ts;

    if (timespec_get(&ts, TIME_UTC) != TIME_UTC)
    {
        fputs("timespec_get failed!", stderr);
        return 0;
    }
    return (uint64_t)(1000000000 * ts.tv_sec) + (uint64_t)ts.tv_nsec;
}

Timer new_timer(uint64_t d) {
    return (Timer){
        .expired = false,
            .duration = d,
            .start = now(),
    };
}

bool update_timer(Timer t) {

    // B_INFO("time: %lld", now());

    if (t.start + t.duration < now()) {
        t.expired = true;
    }
    return t.expired;
}

void wait(Timer t) {
    while (!update_timer(t));
}