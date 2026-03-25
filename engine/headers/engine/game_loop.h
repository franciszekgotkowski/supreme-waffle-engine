#pragma once

#include <engine/memory_pool.h>
#include <stdbool.h>

// checks if os sstem want close window (for example because user clicked on x button or clicked alt-f4)
bool doesSystemWantToCloseWindow();

// clears screen
void clearScreen();

// updates buffer
void updateBuffer();

// polls events (like input, resizing window etc.)
void handleEngineEvents();