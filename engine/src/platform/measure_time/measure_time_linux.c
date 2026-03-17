#include <engine/typedefs.h>
#include <engine/memory_pool.h>
#include <engine/platform/measure_time.h>

#include <assert.h>
#include <time.h>

extern PointerTable* GameMemory;

inline TimeStamp InitializeTimeStamp() {
	struct timespec spec;
	i32 errno = clock_gettime(CLOCK_MONOTONIC, &spec);
	assert(errno == 0);

	return (TimeStamp){
		.sec = spec.tv_sec,
		.nsec = spec.tv_nsec
	};
}

inline DoubleTimeStamp InitializeDoubleTimeStamp(){
	struct timespec spec;
	i32 errno = clock_gettime(CLOCK_MONOTONIC, &spec);
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
	i32 errno = clock_gettime(CLOCK_MONOTONIC, &spec);
	assert(errno == 0);

	ptr->then = ptr->now;
	ptr->now = (TimeStamp) {
		.sec = spec.tv_sec,
		.nsec = spec.tv_nsec
	};

	return;
}

inline TimeStamp TimeSince(TimeStamp stamp) {
		struct timespec now;
		i32 errno = clock_gettime(CLOCK_MONOTONIC, &now);
		assert(errno == 0);
		TimeStamp nowTimeStamp = {
			.sec = now.tv_sec,
			.nsec = now.tv_nsec
		};
		// checking if stamp is not from future
		return TimeDiff(nowTimeStamp, stamp);
}

inline void SleepTime(TimeStamp amount) {
	TimeStamp errorMargin = {
		.sec = 0,
		.nsec = 2000000
	};

	struct timespec functionStart;
	clock_gettime(CLOCK_MONOTONIC, &functionStart);
	TimeStamp functionStartStamp = {
		.sec = functionStart.tv_sec,
		.nsec = functionStart.tv_nsec,
	};
	TimeStamp final = (TimeStamp)AddTimestamps(amount, functionStartStamp);

	struct timespec newTime = {
		.tv_sec = amount.sec,
		.tv_nsec = amount.nsec
	};
	if (SmallerTimeStamp(amount, errorMargin) == 1) {
		newTime.tv_nsec -= errorMargin.nsec;
		struct timespec elapsed;
		nanosleep(&newTime, &elapsed);
	}

	TimeStamp now;
	while (1) {
		clock_gettime(CLOCK_MONOTONIC, (struct timespec*)&now);
		if (SmallerTimeStamp(final, now) == -1) {
			break;
		}
	}
}