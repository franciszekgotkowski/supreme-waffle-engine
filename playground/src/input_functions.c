#include <engine/memory_pool.h>
#include <engine/window.h>
#include <game/input_functions.h>
#include <engine/handle_input.h>

#include <assert.h>
#include <stdio.h>

extern PointerTable* GameMemory;

void CheckForExit() {
	PointerTable* const table = GameMemory;
	assert(table);

	if (KeyJustReleased(K_ESC)) {
		SetWindowToClose();
	}
}

void MouseMoved() {
	PointerTable* const table = GameMemory;
	assert(table);
	mouseState* mouse = &(((InputData*)getRegion(INPUT_DATA))->mouse);
	assert(mouse);


	if(
		mouse->dx != 0.0 ||
		mouse->dy != 0.0
	) {
		printf("mouse moved from (%f, %f) to (%f, %f)", mouse->x, mouse->y, mouse->x - mouse->dx, mouse->y - mouse->dy);
		printf("\t moved by (%f, %f)\n", mouse->dx, mouse->dy);
	}

	// this has to run on the end
	mouse->dx = 0.0;
	mouse->dy = 0.0;
}




void InsertInputFunctions() {
	PushInputFunction(CheckForExit);
	PushInputFunction(MouseMoved);
}

void handleInput() {
	RunAllInputFunctions();
}