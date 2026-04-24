#pragma once

#include <common/typedefs.h>
#include <common/errors.h>
#include <stdbool.h>

typedef u64 CheckpointID;

#define MAX_AMOUNT_OF_ARENA_CHECKPOINTS 100
typedef struct {
	void* checkpoint[MAX_AMOUNT_OF_ARENA_CHECKPOINTS]; // chekcpoints are places to which memory arena can reset to without reseting its all memory
	u64 amountOfCheckpoints;
	void* base; // base pointer to arena allocation region
	void* top; // current top of allocated memory
	u64 capacity; // maximum capacity of an arena including this struct
	bool locked; // if memory arena is locked, pushing or reseting memory is illegal and functions should return an error
} MemoryArena ;

// Initializes memory arena on base pointer.
// Arena will occupy cap amount of bytes, but usable space will be a little bit lower because of MemoryArena struct
// Can return errors:
// 	- OK
// 	- OUT_OF_MEMORY		if memory given for the arena is smaller that MemoryArena struct
Error InitializeMemoryArena(
	void* base,
	u64 cap
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

// Will crash if there is to little space.
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
// Also each sequentially added checkpoint has to have higher address than the last one. This is because when you reset to checkpoint memory checkpoints above the selected checkpoint have to be removed. If checkpoints did't have increasing adresses after reseting arena, there could be risk that existing chekcpoints point to non-existant memory
// Can return errors:
// 	- OK
// 	- LOCKED			if memory arena is locked
// 	- OUT_OF_INDEXES	if there is no more available memory checkpoints to use
// 	- OUT_OF_RANGE		if ptr does not belong to allocated adress space inside arena
// 	- INVALID_INPUT		if ptr new pointer has lower value that the pointer from checkpoint earlier
CheckpointID addCheckpoint_MemoryArena(
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
	CheckpointID id
);