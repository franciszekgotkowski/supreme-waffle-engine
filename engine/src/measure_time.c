#include <engine/platform/measure_time.h>

#include <stdio.h>
#include <assert.h>

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

inline TimeStamp PrintTimeSince(TimeStamp stamp) {
	TimeStamp diff = TimeSince(stamp);
	printf("Time elapsed: %llus %llums\n", (llu)diff.sec, (llu)diff.nsec);
	return diff;
}

inline i8 SmallerTimeStamp(TimeStamp a, TimeStamp b) {
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

inline void MatchFrametime(TimeStamp frameTime, TimeStamp lastStamp) {
	TimeStamp elapsed = TimeSince(lastStamp);

	if (SmallerTimeStamp(elapsed, frameTime) == 1) {
		printf("FRAMETIME MISSED!!!\tFRAME TOOK %llus %lluns\n", (llu)elapsed.sec, (llu)elapsed.nsec);
		return;
	}

	TimeStamp timeToWait = TimeDiff(frameTime, elapsed);
	SleepTime(timeToWait);
}