#include "engine/platform/measure_time.h"
#include <assert.h>
#include <stdio.h>

#include <engine/platform/input.h>
#include <engine/memory_pool.h>
#include <engine/platform/window_data.h>
#include <engine/errors.h>

#include <external/glad/glad.h>
#include <external/glfw3.h>

extern PointerTable* GameMemory;

void inputCallback(
	GLFWwindow* window,
	i32 key,
	i32 scancode,
	i32 action,
	i32 mods
);

static void framebuffer_size_callback(
	GLFWwindow *window,
	i32 width,
	i32 height
) {
	assert(window);
	assert(GameMemory);

	WindowData* const windowData = GameMemory->regions[WINDOW_DATA].ptr;

	printf("window is resized from %dx%d to %dx%d\n", windowData->windowHeight, windowData->windowWidth, height, width);

    windowData->windowWidth = width;
    windowData->windowHeight = height;

    glViewport(0, 0, windowData->windowWidth, windowData->windowHeight);
}

static inline void window_should_close_callback(
	GLFWwindow* window
) {
	assert(window);
	((WindowData*)getRegion(WINDOW_DATA))->windowShouldClose = true;
}

void InitializeWindow(
	i32 windowWidth,
	i32 windowHeight,
	i32 fps,
	bool vsync,
	bool resizable,
	v4 clearColor,
	str title,
	CursorMode cursorMode
) {
	WindowData* const windowData = getRegion(WINDOW_DATA);
    assert(windowData);
    assert(title);
    assert(windowWidth > 0 && windowHeight > 0 && fps > 0);

    i32 cursorModeGlfw;
    switch (cursorMode) {
   		case CURSOR_NORMAL:
    		cursorModeGlfw = GLFW_CURSOR_NORMAL;
     		break;
       	case CURSOR_HIDDEN:
       		cursorModeGlfw = GLFW_CURSOR_HIDDEN;
        	break;
        case CURSOR_DISABLED:
       		cursorModeGlfw = GLFW_CURSOR_DISABLED;
        	break;
        default:
       		cursorModeGlfw = GLFW_CURSOR_NORMAL;
         	break;
    }

    TimeStamp t = InitializeTimeStamp();

    (*windowData) = (WindowData){
        .window = NULL,
        .windowHeight = windowHeight,
        .windowWidth = windowWidth,
        .fps = fps,
        .frametime = CalculateFrametime(fps),
        .cursorMode = cursorModeGlfw,
        .vsync = vsync,
        .resizable = resizable,
        .windowShouldClose = false,
        .clearColor = clearColor,
        .framesElapsed = 0,
        .bootTimeStamp = t,
        .frametimeEveningTimeStamp = t,
    };

    // if (!glfwInit()) {return LIBRARY_FAIL;}
    assert(glfwInit());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

    windowData->window = glfwCreateWindow(windowWidth, windowHeight, title, NULL, NULL);
    assert(windowData->window);

    glfwMakeContextCurrent(windowData->window);
    glfwSetInputMode(windowData->window, GLFW_CURSOR, cursorModeGlfw);

    glfwSwapInterval(vsync);

    assert(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress));

    glfwGetFramebufferSize(windowData->window, &(windowData->windowWidth), &(windowData->windowHeight));
    glViewport(0, 0, windowData->windowWidth, windowData->windowHeight);
    glClearColor(windowData->clearColor.arr[0],windowData->clearColor.arr[1], windowData->clearColor.arr[2], windowData->clearColor.arr[3]);

    glfwSetFramebufferSizeCallback(windowData->window, framebuffer_size_callback);
    glfwSetWindowCloseCallback(windowData->window, window_should_close_callback);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwPollEvents();
}

void SetWindowToClose() {
	WindowData* const window = getRegion(WINDOW_DATA);
	assert(window);
	window->windowShouldClose = true;
}

void CloseWindow() {
    glfwTerminate();
}

