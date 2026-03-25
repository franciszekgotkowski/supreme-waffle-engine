#include <export/platform/window_data.h>
#include <assert.h>
#include <stdbool.h>

#include <primitives/errors.h>
#include <primitives/range.h>
#include <export/platform/input.h>
#include <external/glfw3.h>

extern InputData* inputData;
extern WindowData* windowData;

static Error updateKeyboard(
	i32 key,
	i32 scancode,
	i32 action,
	i32 mods
) {

	bool keyNow = false;
 	bool keyPast = false;
	if (action == GLFW_PRESS) {
		keyNow = 1;
		keyPast = 0 ;
	}
	if (action == GLFW_RELEASE) {
		keyNow = 0;
		keyPast = 1 ;
	}

	if (mods == GLFW_MOD_ALT) {
		inputData->keyboard.pressedPreviously[K_ALT] = inputData->keyboard.pressedNow[K_ALT];
		inputData->keyboard.pressedNow[K_ALT] = true;
	} else {
		inputData->keyboard.pressedPreviously[K_ALT] = inputData->keyboard.pressedNow[K_ALT];
		inputData->keyboard.pressedNow[K_ALT] = false;
	}

	if (mods == GLFW_MOD_CONTROL) {
		inputData->keyboard.pressedPreviously[K_CTRL] = inputData->keyboard.pressedNow[K_CTRL];
		inputData->keyboard.pressedNow[K_CTRL] = true;
	} else {
		inputData->keyboard.pressedPreviously[K_CTRL] = inputData->keyboard.pressedNow[K_CTRL];
		inputData->keyboard.pressedNow[K_CTRL] = false;
	}

	if (mods == GLFW_MOD_SHIFT) {
		inputData->keyboard.pressedPreviously[K_SHIFT] = inputData->keyboard.pressedNow[K_SHIFT];
		inputData->keyboard.pressedNow[K_SHIFT] = true;
	} else {
		inputData->keyboard.pressedPreviously[K_SHIFT] = inputData->keyboard.pressedNow[K_SHIFT];
		inputData->keyboard.pressedNow[K_SHIFT] = false;
	}

	if (mods == GLFW_MOD_SUPER) {
		inputData->keyboard.pressedPreviously[K_SUPER] = inputData->keyboard.pressedNow[K_SUPER];
		inputData->keyboard.pressedNow[K_SUPER] = true;
	} else {
		inputData->keyboard.pressedPreviously[K_SUPER] = inputData->keyboard.pressedNow[K_SUPER];
		inputData->keyboard.pressedNow[K_SUPER] = false;
	}

	switch (key) {
		case GLFW_KEY_ENTER:
			inputData->keyboard.pressedNow[K_ENTER] = keyNow;
			inputData->keyboard.pressedPreviously[K_ENTER] = keyPast;
			break;
		case GLFW_KEY_ESCAPE:
			inputData->keyboard.pressedNow[K_ESC] = keyNow;
			inputData->keyboard.pressedPreviously[K_ESC] = keyPast;
			break;
	}

	return OK;
}

// // GAMEPAD IS NOT IMPLEMENTED
// static Error updateGamepad(PointerTable* table){
// 	return OK;
// }



void inputCallback(
	GLFWwindow* window,
	i32 key,
	i32 scancode,
	i32 action,
	i32 mods
) {
	assert(window);

	Error err = updateKeyboard(key, scancode, action, mods);
	assert(err == OK);
}

void mouseCallback(
	GLFWwindow *window,
	f64 xpos,
	f64 ypos
) {
	assert(window);

	mouseState* mouse = &(inputData->mouse);
	assert(inputData);
	assert(mouse);

	mouse->dx = xpos - mouse->x;
	mouse->dy = ypos - mouse->y;

	mouse->x = xpos;
	mouse->y = ypos;

}

void InitializeInputData() {
	assert(inputData);
	assert(windowData);

	*inputData = (InputData){};
	glfwSetKeyCallback(windowData->window, inputCallback);
    glfwSetCursorPosCallback(windowData->window, mouseCallback);
    // glfwSetScrollCallback(window, scroll_callback);
}