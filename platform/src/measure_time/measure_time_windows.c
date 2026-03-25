#include <stdio.h>
#include <engine/platform/measure_time.h>
#include <engine/typedefs.h>

#include <windows.h>

typedef struct {
    LARGE_INTEGER frequency;
    LARGE_INTEGER then;
    LARGE_INTEGER now;
    f64 diff; // is filled when time_elapsed is run so that wait_for_frame does not have to calculate diff again
} win_stamp;
_Static_assert(sizeof(win_stamp)==sizeof(timestamp), "lin_stamp has different size from timestamp");

inline timestamp make_stamp() {
    /*
    timestamp:
    [8byte] - frequency
    [8byte] - then
    [8byte] - now
    [8byte] - diff
    */

    win_stamp stamp = {0};
    QueryPerformanceFrequency(&(stamp.frequency));
    QueryPerformanceCounter(&(stamp.now));
    timestamp t;
    memcpy(&t, &stamp, sizeof(stamp));
    return t;
}

// returns time difference in miliseconds
inline static f64 time_diff(LARGE_INTEGER then, LARGE_INTEGER now, LARGE_INTEGER freq) {
    return (1000.0*(f64)(now.QuadPart - then.QuadPart))/(f64)freq.QuadPart;
}

inline f64 elapsed_time(timestamp *ptr) {
    win_stamp* stamp = (win_stamp*)ptr;

    stamp->then = stamp->now;
    QueryPerformanceCounter(&(stamp->now));
    stamp->diff = time_diff(stamp->then, stamp->now, stamp->frequency);
    return stamp->diff;
}

inline f64 read_elapsed_time(timestamp* ptr) {
    win_stamp* stamp = (win_stamp*)ptr;
    return stamp->diff;
}

inline void wait_for_frame() {
    win_stamp* stamp = (win_stamp*)&(((WindowData*)getRegion(WINDOW_DATA))->fpsMaintainingTimestamp);
    i32 fps = ((WindowData*)getRegion(WINDOW_DATA))->fps;

    f64 timePerFrame = 1000.0/fps;
    f64 timeToWait = timePerFrame - stamp->diff;
    if (timeToWait < 0) { timeToWait = 0; }
    Sleep((u64)timeToWait);
}