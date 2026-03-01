#include "engine/typedefs.h"
#include <bits/time.h>
#include <engine/memory_pool.h>
#include <engine/platform/measure_time.h>

#include <assert.h>
#include <stdio.h>
#include <time.h>

extern PointerTable* GameMemory;

inline TimeStamp InitializeTimeStamp() {
	struct timespec spec;
	i32 errno = clock_gettime(CLOCK_REALTIME, &spec);
	assert(errno == 0);

	return (TimeStamp){
		.sec = spec.tv_sec,
		.nsec = spec.tv_nsec
	};
}

inline DoubleTimeStamp InitializeDoubleTimeStamp(){
	struct timespec spec;
	i32 errno = clock_gettime(CLOCK_REALTIME, &spec);
	assert(errno == 0);

	return (DoubleTimeStamp){
		.then = {
			.sec = spec.tv_sec,
			.nsec = spec.tv_nsec
		}
	};
}

inline void UpdateDoubleTimeStamp(DoubleTimeStamp *ptr) {
	assert(ptr);

	struct timespec spec;
	i32 errno = clock_gettime(CLOCK_REALTIME, &spec);
	assert(errno == 0);

	ptr->then = ptr->now;
	ptr->now = (TimeStamp) {
		.sec = spec.tv_sec,
		.nsec = spec.tv_nsec
	};

	return;
}

// a - b
inline TimeStamp TimeDiff(TimeStamp a, TimeStamp b) {
	// checks if a > b
	assert(
		b.sec < a.sec ||
		(b.sec == a.sec && b.nsec < a.nsec)
	);

	TimeStamp timeDiff = {};

	if (a.nsec < b.nsec) {
		u64 nsecDiff = b.nsec - a.nsec;

		timeDiff.sec = a.sec - b.sec - 1;
		timeDiff.nsec = 1000000000 - nsecDiff;
	} else {
		timeDiff.nsec = a.nsec - b.nsec;
		timeDiff.sec = a.sec - b.sec;
	}

	return timeDiff;
}

inline TimeStamp TimeSince(TimeStamp stamp) {
		struct timespec now;
		i32 errno = clock_gettime(CLOCK_REALTIME, &now);
		assert(errno == 0);
		TimeStamp nowTimeStamp = {
			.sec = now.tv_sec,
			.nsec = now.tv_nsec
		};
		// checking if stamp is not from future
		return TimeDiff(nowTimeStamp, stamp);
}
inline TimeStamp PrintTimeSince(TimeStamp stamp) {
	TimeStamp diff = TimeSince(stamp);
	printf("Time elapsed: %llus %llums\n", (llu)diff.sec, (llu)diff.nsec);
	return diff;
}

inline i8 CompareTimeStamps(TimeStamp a, TimeStamp b) {
	if (a.sec < b.sec) {
		return -1;
	} else if (a.sec > b.sec) {
		return 1;
	} else {
		if (a.nsec < b.nsec) {
			return -1;
		} else if (a.nsec > b.nsec) {
			return 1;
		} else {
			return 0;
		}
	}
}

inline TimeStamp AddTimestamps(TimeStamp a, TimeStamp b) {
	TimeStamp stamp = {
		.sec = a.sec + b.sec,
		.nsec = a.nsec + b.nsec
	};

	if (stamp.nsec >= 1000000000) {
		stamp.nsec -= 1000000000;
		stamp.sec++;
	}

	return stamp;
}

inline void SleepTime(TimeStamp amount) {
	TimeStamp errorMargin = {
		.sec = 0,
		.nsec = 2000000
	};

	struct timespec functionStart;
	clock_gettime(CLOCK_REALTIME, &functionStart);
	TimeStamp functionStartStamp = {
		.sec = functionStart.tv_sec,
		.nsec = functionStart.tv_nsec,
	};
	TimeStamp final = (TimeStamp)AddTimestamps(amount, functionStartStamp);

	struct timespec newTime = {
		.tv_sec = amount.sec,
		.tv_nsec = amount.nsec
	};
	if (CompareTimeStamps(amount, errorMargin) == 1) {
		newTime.tv_nsec -= errorMargin.nsec;
		struct timespec elapsed;
		nanosleep(&newTime, &elapsed);
	}

	TimeStamp now;
	while (1) {
		clock_gettime(CLOCK_REALTIME, (struct timespec*)&now);
		if (CompareTimeStamps(final, now) == -1) {
			break;
		}
	}
}

inline void MatchFrametime(TimeStamp frameTime, TimeStamp lastStamp) {
	TimeStamp elapsed = TimeSince(lastStamp);

	if (CompareTimeStamps(elapsed, frameTime) == 1) {
		printf("FRAMETIME MISSED!!!\tFRAME TOOK %llus %lluns\n", (llu)elapsed.sec, (llu)elapsed.nsec);
		return;
	}

	TimeStamp timeToWait = TimeDiff(frameTime, elapsed);
	SleepTime(timeToWait);
}

inline TimeStamp CalculateFrametime(u64 fps) {
	assert(fps > 0);

	if (fps == 1) {
		return (TimeStamp){
			.sec = 1,
			.nsec = 0
		};
	}

	return (TimeStamp){
		.sec = 0,
		.nsec = 1000000000 / fps
	};
}
