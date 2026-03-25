#include <export/platform/input.h>
#include <export/platform/window_data.h>
#include <primitives/errors.h>
#include <export/platform/window_data.h>
#include <assert.h>

#include <glad/glad.h>
#include <glfw3.h>

extern WindowData* windowData;
extern InputData* inputData;

bool doesSystemWantToCloseWindow() {
	assert(windowData);
	return glfwWindowShouldClose(windowData->window);
}

void updateBuffer() {
	assert(windowData);
	glfwSwapBuffers(windowData->window);
}

void handleEngineEvents() {
	glfwPollEvents();
}