#pragma once

#include <primitives/errors.h>
#include <primitives/typedefs.h>

#include <stdbool.h>

typedef enum {
	K_A,	K_B,	K_C,	K_D,
	K_E,	K_F,	K_G,	K_H,
	K_I,	K_J,	K_K,	K_L,
	K_M,	K_N,	K_O,	K_P,
	K_Q,	K_R,	K_S,	K_T,
	K_U,	K_V,	K_W,	K_X,
	K_Y,	K_Z,	K_0,	K_1,
	K_2,	K_3,	K_4,	K_5,
	K_6,	K_7,	K_8,	K_9,
	K_SPACE,	K_ENTER,	K_BACKSPACE,	K_ESC,
	K_SHIFT,	K_CTRL,		K_ALT,		K_SUPER,

	KEYBOARD_KEYS_AMOUNT
} KeyboardKeys ;

typedef enum {
	M_LEFT,
    M_RIGHT,
    M_MIDDLE,
    MB_4, // Often "Back"
    MB_5, // Often "Forward"
    MB_6,
    MB_7,
    MB_8,

    MOUSE_BUTTONS_AMOUNT
} MouseInput ;

#define MAX_KEYBOARD_BUTTONS_AMOUNT 1024
#define MAX_MOUSE_BUTTONS_AMOUNT 8

// typedef struct {
// 	bool pressedNow[MAX_KEYBOARD_BUTTONS_AMOUNT];
//     bool pressedPreviously[MAX_KEYBOARD_BUTTONS_AMOUNT];
// } keyboardState ;
typedef struct {
	bool pressedNow[KEYBOARD_KEYS_AMOUNT];
    bool pressedPreviously[KEYBOARD_KEYS_AMOUNT];
} keyboardState ;

// typedef struct {
// 	bool pressedNow[MAX_MOUSE_BUTTONS_AMOUNT];
//     bool pressedPreviously[MAX_MOUSE_BUTTONS_AMOUNT];

//     f64 x, y;
//     f64 dx, dy;
//     f64 scroll;
// } mouseState ;
typedef struct {
	bool pressedNow[MOUSE_BUTTONS_AMOUNT];
    bool pressedPreviously[MOUSE_BUTTONS_AMOUNT];

    f64 x, y;
    f64 dx, dy;
    f64 scroll;
} mouseState ;

typedef struct {

} gamepadState ;

typedef struct {
	keyboardState keyboard;
	mouseState mouse;
	gamepadState gamepad;
} InputData ;

#define MAXIMUM_INPUT_FUNCTIONS_AMOUNT 1024
typedef void (*InputFunction)() ;
// Functions can be added to input qeue with PushInputFunction. They will be ran every frame in handleInput() function.
typedef struct {
	u64 amountOfFunctions;
	InputFunction function[MAXIMUM_INPUT_FUNCTIONS_AMOUNT];
} InputFunctions;


// checks if key have just been pressed
bool KeyJustPressed(
	KeyboardKeys key
);

// ckecks if key is pressed now and was in previous frame
bool KeyLongPressed(
	KeyboardKeys key
);

// checks if key is pressed (does not consider previous frame)
bool KeyIsPressed(
	KeyboardKeys key
);

// checks if key has just been released
bool KeyJustReleased(
	KeyboardKeys key
);

void InitializeInputData();

// This function sets input function callbacks for game to run every frame.
// Functions will look at input and then do something
void InitializeInputFunctions();

void PushInputFunction(
	InputFunction func
);

void PopInputFunction();

void ClearInputFunction();

void RunAllInputFunctions();