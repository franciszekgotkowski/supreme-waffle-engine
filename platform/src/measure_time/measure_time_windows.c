#include <assert.h>
#include <common/typedefs.h>
#include <stdbool.h>
#include <export/platform/measure_time.h>

#include <windows.h>

typedef struct {
    LARGE_INTEGER frequency;
    LARGE_INTEGER performanceCounts;
} WinStamp;

_Static_assert(sizeof(WinStamp)==sizeof(TimeStamp), "lin_stamp has different size from timestamp");

bool latencyMinimized = false;
inline static void MinimizeLatency() {
    if (!latencyMinimized) {
        timeBeginPeriod(1);
        latencyMinimized = true;
    }
}

TimeStamp InitializeTimeStamp() {

    MinimizeLatency();

    LARGE_INTEGER freq, ticks;

    bool ok = QueryPerformanceCounter( (LARGE_INTEGER*)&ticks );
    assert( ok );
    ok = QueryPerformanceFrequency( (LARGE_INTEGER*)&freq );
    assert( ok );

    TimeStamp ret = {
        .sec = ticks.QuadPart / freq.QuadPart,
        .nsec = ((ticks.QuadPart % freq.QuadPart) * 1000000000LL) / freq.QuadPart
    };

    return ret;
}

void SleepTime(TimeStamp amount) {
    MinimizeLatency();

    TimeStamp errorMargin = {
        .sec = 0,
        .nsec = 2000000
    };


    TimeStamp functionStartStamp = InitializeTimeStamp();
    TimeStamp final = (TimeStamp)AddTimestamps(amount, functionStartStamp);

    TimeStamp newTime = amount;

    if (SmallerTimeStamp(amount, errorMargin) == 1) {
        newTime.nsec -= errorMargin.nsec;
        assert(newTime.nsec + newTime.sec * 1000000000 < UINT32_MAX);
        DWORD msToSleep = newTime.nsec + newTime.sec * 1000000000;
        Sleep(msToSleep);
    }

    while (true) {
        TimeStamp now = InitializeTimeStamp();
        if (SmallerTimeStamp(final, now) == -1) {
            break;
        }
    }
}
