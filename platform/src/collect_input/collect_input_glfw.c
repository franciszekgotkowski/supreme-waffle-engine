#include <export/platform/window.h>
#include <assert.h>
#include <stdbool.h>
#include <common/errors.h>
#include <common/range.h>
#include <common/keycodes.h>
#include <export/platform/collect_input.h>
#include <external/glfw3.h>


extern InputData* inputData;
extern WindowData* windowData;

int KeycodeTable[GLFW_KEY_LAST+1] = {
	[GLFW_KEY_A] = K_A,
	[GLFW_KEY_B] = K_B,
	[GLFW_KEY_C] = K_C,
	[GLFW_KEY_D] = K_D,
	[GLFW_KEY_E] = K_E,
	[GLFW_KEY_F] = K_F,
	[GLFW_KEY_G] = K_G,
	[GLFW_KEY_H] = K_H,
	[GLFW_KEY_I] = K_I,
	[GLFW_KEY_J] = K_J,
	[GLFW_KEY_K] = K_K,
	[GLFW_KEY_L] = K_L,
	[GLFW_KEY_M] = K_M,
	[GLFW_KEY_N] = K_N,
	[GLFW_KEY_O] = K_O,
	[GLFW_KEY_P] = K_P,
	[GLFW_KEY_Q] = K_Q,
	[GLFW_KEY_R] = K_R,
	[GLFW_KEY_S] = K_S,
	[GLFW_KEY_T] = K_T,
	[GLFW_KEY_U] = K_U,
	[GLFW_KEY_V] = K_V,
	[GLFW_KEY_W] = K_W,
	[GLFW_KEY_X] = K_X,
	[GLFW_KEY_Y] = K_Y,
	[GLFW_KEY_Z] = K_Z,
	[GLFW_KEY_0] = K_0,
	[GLFW_KEY_1] = K_1,
	[GLFW_KEY_2] = K_2,
	[GLFW_KEY_3] = K_3,
	[GLFW_KEY_4] = K_4,
	[GLFW_KEY_5] = K_5,
	[GLFW_KEY_6] = K_6,
	[GLFW_KEY_7] = K_7,
	[GLFW_KEY_8] = K_8,
	[GLFW_KEY_9] = K_9,
	[GLFW_KEY_SPACE] = K_SPACE,
	[GLFW_KEY_ENTER] = K_ENTER,
	[GLFW_KEY_BACKSPACE] = K_BACKSPACE,
	[GLFW_KEY_ESCAPE] = K_ESC,
};

int KeyModsTable[] = {
	[GLFW_MOD_SHIFT] = K_MOD_SHIFT,
	[GLFW_MOD_CONTROL] = K_MOD_CTRL,
	[GLFW_MOD_ALT] = K_MOD_ALT,
	[GLFW_MOD_SUPER] = K_MOD_SUPER,
	[GLFW_MOD_CAPS_LOCK] = K_MOD_CAPS_LOCK,
	[GLFW_MOD_NUM_LOCK] = K_MOD_NUM_LOCK,
};

static void updateKeyboard(
	i32 key,
	i32 scancode,
	i32 action,
	i32 mods
) {
	bool keyNow;
 	bool keyPast;
	if (action == GLFW_PRESS) {
		keyNow = 1;
		keyPast = 0 ;
	}
	if (action == GLFW_RELEASE) {
		keyNow = 0;
		keyPast = 1 ;
	}

	if (KeycodeTable[key] != 0) {
		inputData->keyboard.pressedNow[KeycodeTable[key]] = keyNow;
		inputData->keyboard.pressedPreviously[KeycodeTable[key]] = keyPast;
	}
	inputData->keyboard.mods = mods;
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

	updateKeyboard(key, scancode, action, mods);
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

void InitializeInputCallbacks() {
	assert(inputData);
	assert(windowData);

	*inputData = (InputData){};
	glfwSetKeyCallback(windowData->window, inputCallback);
	glfwSetCursorPosCallback(windowData->window, mouseCallback);
	// glfwSetScrollCallback(window, scroll_callback);
}