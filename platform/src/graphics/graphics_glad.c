#include <export/platform/window_data.h>

#include <external/glad/glad.h>

#include <assert.h>

extern WindowData* windowData;

void clearScreen() {
	glClear(GL_COLOR_BUFFER_BIT);
}

// function that gets v2u32 with pixel offsets and transforms them into screenspace v2 that starts at (0, 0)
inline v2 PixelsToScreenspace(v2u32 px) {
	assert(windowData);
	assert(px.x < windowData->windowWidth);
	assert(px.y < windowData->windowHeight);
	v2 ret = {
		.x = (f32)( (i32)px.x - windowData->windowWidth/2 ) / (f32)(windowData->windowWidth/2),
		.y = (f32)( (i32)px.y - windowData->windowHeight/2 ) / (f32)(-windowData->windowHeight/2),
	};

	return ret;
}

