#pragma once

#include <engine/platform/measure_time.h>
#include <engine/typedefs.h>
#include <engine/errors.h>
#include <engine/memory_pool.h>

#include <stdbool.h>

typedef enum {
	CURSOR_NORMAL,
	CURSOR_HIDDEN,
	CURSOR_DISABLED
} CursorMode;

typedef struct {
    void* window;
    i32 windowWidth;
    i32 windowHeight;
    i32 fps;
    i32 cursorMode;
    bool vsync;
    bool resizable;
    bool windowShouldClose;
    v4 clearColor;

    TimeStamp frametime;
    u64 framesElapsed;
    TimeStamp bootTimeStamp;
    TimeStamp frametimeEveningTimeStamp;
} WindowData ;

void InitializeWindow(
	i32 windowWidth,
    i32 windowHeight,
	i32 fps,
	bool vsync,
	bool resizable,
	v4 clearColor,
	str title,
	CursorMode cursorMode
);

// Sets windowShouldClose parameter to true
void SetWindowToClose();

void CloseWindow();

void GameLoop();