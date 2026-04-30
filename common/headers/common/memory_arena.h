#pragma once

#include <common/typedefs.h>
#include <common/errors.h>
#include <stdbool.h>

#define MAX_AMOUNT_OF_ARENA_CHECKPOINTS 100
typedef struct {
	void* checkpoint[MAX_AMOUNT_OF_ARENA_CHECKPOINTS]; // chekcpoints are places to which memory arena can reset to without reseting its all memory
	u64 amountOfCheckpoints;
	void* base; // base pointer to arena allocation region
	void* top; // current top of allocated memory
	u64 capacity; // maximum capacity of an arena including this struct
	bool locked; // if memory arena is locked, pushing or reseting memory is illegal and functions should return an error
} MemoryArena ;

// Returns memory arena struct prepared for adding memory
// ptr is starting address of allocation memory space, it can be in arbitrary place relative to the returned struct
MemoryArena InitializeMemoryArena(
	void* ptr,	// starting adress
	u64 cap		// amount of space in arena
);

// Allocates space for memory but not put any data in allocated memory region
// Can return error:
//	- OK 				if everything went well
//	- OUT_OF_MEMORY		if there is no memory to accomodate this one
//	- LOCKED			if memory arena is locked
void* registerMemory_MemoryArena(
	MemoryArena* arena,
	u64 size,
	Error* err
);

// Push will increase head of arena.
// Function returns pointer to where variable now resides
// If input is nullptr no data will be copied!
// Can return error:
//	- OK 				if everything went well
//	- OUT_OF_MEMORY		if there is no memory to accomodate this one
//	- LOCKED			if memory arena is locked
void* registerAndInitializeMemory_MemoryArena(
	MemoryArena* arena,
	u64 size,
	void* input, // pointer to data to be copied
	Error* err
);

// Resets arena to state with no memory allocated.
// Can return errors:
// 	- OK
// 	- LOCKED			if memory arena is locked
Error reset_MemoryArena(
	MemoryArena* arena
);

// Adds checkpoint if possible and return its ID for later reference
// Also each sequentially added checkpoint has to have higher address than the last one. This is because when you reset to checkpoint memory checkpoints above the selected checkpoint have to be removed. If checkpoints did't have increasing adresses after reseting arena, there could be risk that existing checkpoint would point to non-allocated memory
// Can return errors:
// 	- OK
// 	- LOCKED			if memory arena is locked
// 	- OUT_OF_INDEXES	if there is no more available memory checkpoints to use
// 	- OUT_OF_RANGE		if ptr does not belong to allocated adress space inside arena
// 	- INVALID_INPUT		if ptr new pointer has lower value that the pointer from checkpoint earlier
ID addCheckpoint_MemoryArena(
	MemoryArena* arena,
	void* ptr,
	Error* err
);


// Resets memory arena not to 0 but to the selected checpoint
// Can return errors:
// 	- OK
// 	- OUT_OF_RANGE		if there is no checkpoint under inputted ID
// 	- LOCKED 			if arena is locked
Error resetToCheckpoint_MemoryArena(
	MemoryArena* arena,
	ID id
);

void lock_MemoryArena(
	MemoryArena* arena
);

void unlock_MemoryArena(
	MemoryArena* arena
);