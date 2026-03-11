#pragma once

#include <engine/typedefs.h>
#include <engine/errors.h>
#include <stdbool.h>

#define MAX_AMOUNT_OF_ARENA_CHECKPOINTS 100
typedef struct {
	void* checkpoint[MAX_AMOUNT_OF_ARENA_CHECKPOINTS];
	u64 amountOfCheckpoints;
	void* base; // base pointer to arena
	void* top; // current top of allocated memory
	u64 capacity; // maximum capacity of an arena including this struct
	bool locked; // if memory arena is locked, pushing or reseting memory is illegal and functions should return an error
} MemoryArena ;

// Initializes memory arena on base pointer.
// Arena will occupy cap amount of bytes, but usable space will be a littple bit loweb because of MemoryArena struct
Error InitializeMemoryArena(
	void* base,
	u64 cap
);

// Will crash if there is to little space.
// Push will increase head of arena.
// Function returns pointer to where variable now resides
//
// if input is nullptr no data will be copied!
void* push_MemoryArena(
	MemoryArena* arena,
	u64 size,
	void* input, // pointer to data to be copied
	Error* err
);

Error reset_MemoryArena(
	MemoryArena* arena
);

Error addCheckpoint_MemoryArena(
	MemoryArena* arena,
	void* ptr
);

Error returnToCheckpoint_MemoryArena(
	MemoryArena* arena,
	u64 idx
);