#include <engine/platform/measure_time.h>
#include <windows.h>
#include <assert.h>

#include <engine/platform/measure_time.h>

#include <assert.h>
#include <windows.h>

#define NSEC_PER_SEC 1000000000ull

static LARGE_INTEGER Win32Frequency(void) {
    static LARGE_INTEGER freq = {0};
    if (freq.QuadPart == 0) {
        QueryPerformanceFrequency(&freq);
    }
    return freq;
}

static TimeStamp Win32Now(void) {
    LARGE_INTEGER counter;
    LARGE_INTEGER freq = Win32Frequency();
    QueryPerformanceCounter(&counter);

    u64 ticks = (u64)counter.QuadPart;
    u64 hz = (u64)freq.QuadPart;

    return (TimeStamp){
        .sec = ticks / hz,
        .nsec = ((ticks % hz) * NSEC_PER_SEC) / hz
    };
}

inline TimeStamp InitializeTimeStamp() {
    return Win32Now();
}

inline DoubleTimeStamp InitializeDoubleTimeStamp() {
    TimeStamp now = Win32Now();
    return (DoubleTimeStamp){
        .then = now
    };
}

inline void UpdateDoubleTimeStamp(DoubleTimeStamp *ptr) {
    assert(ptr);

    ptr->then = ptr->now;
    ptr->now = Win32Now();
}

inline TimeStamp TimeSince(TimeStamp stamp) {
    TimeStamp now = Win32Now();
    return TimeDiff(now, stamp);
}

inline void SleepTime(TimeStamp amount) {
    TimeStamp errorMargin = {
        .sec = 0,
        .nsec = 2000000
    };

    TimeStamp start = Win32Now();
    TimeStamp final = AddTimestamps(amount, start);

    if (SmallerTimeStamp(amount, errorMargin) == 1) {
        u64 ms = amount.sec * 1000ull
                + (amount.nsec - errorMargin.nsec) / 1000000ull;
        if (ms > 0) {
            Sleep((DWORD)ms);
        }
    }

    while (SmallerTimeStamp(Win32Now(), final) == -1) {
        YieldProcessor();
    }
}