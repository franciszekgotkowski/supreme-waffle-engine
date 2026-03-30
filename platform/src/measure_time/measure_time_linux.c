#include <common/typedefs.h>
#include <export/platform/measure_time.h>

#include <assert.h>
#include <time.h>

inline TimeStamp InitializeTimeStamp() {
	struct timespec spec;
	i32 errno = clock_gettime(CLOCK_MONOTONIC, &spec);
	assert(errno == 0);

	return (TimeStamp){
		.sec = spec.tv_sec,
		.nsec = spec.tv_nsec
	};
}
//
// inline DoubleTimeStamp InitializeDoubleTimeStamp(){
// 	return (DoubleTimeStamp){
// 		.then = InitializeTimeStamp(),
// 		.now = {}
// 	};
// }
//
// inline void UpdateDoubleTimeStamp(DoubleTimeStamp *ptr) {
// 	assert(ptr);
//
// 	struct timespec spec;
// 	i32 errno = clock_gettime(CLOCK_MONOTONIC, &spec);
// 	assert(errno == 0);
//
// 	ptr->then = ptr->now;
// 	ptr->now = (TimeStamp) {
// 		.sec = spec.tv_sec,
// 		.nsec = spec.tv_nsec
// 	};
//
// 	return;
// }
//
// inline TimeStamp TimeSince(TimeStamp stamp) {
// 		struct timespec now;
// 		i32 errno = clock_gettime(CLOCK_MONOTONIC, &now);
// 		assert(errno == 0);
// 		TimeStamp nowTimeStamp = {
// 			.sec = now.tv_sec,
// 			.nsec = now.tv_nsec
// 		};
// 		// checking if stamp is not from future
// 		return TimeDiff(nowTimeStamp, stamp);
// }
//
inline void SleepTime(TimeStamp amount) {
	TimeStamp errorMargin = {
		.sec = 0,
		.nsec = 2000000
	};

	TimeStamp functionStartStamp = InitializeTimeStamp();
	TimeStamp final = (TimeStamp)AddTimestamps(amount, functionStartStamp);

	struct timespec newTime = {
		.tv_sec = amount.sec,
		.tv_nsec = amount.nsec
	};
	// if time to sleep is larger than our time margin
	if (SmallerTimeStamp(amount, errorMargin) == 1) {
		newTime.tv_nsec -= errorMargin.nsec;
		struct timespec elapsed;
		nanosleep(&newTime, &elapsed);
	}

	// TimeStamp now;
	while (true) {
		TimeStamp now = InitializeTimeStamp();
		// clock_gettime(CLOCK_MONOTONIC, (struct timespec*)&now);
		if (SmallerTimeStamp(final, now) == -1) {
			break;
		}
	}
}