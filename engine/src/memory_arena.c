#include <engine/typedefs.h>
#include <engine/memory_pool.h>
#include <engine/errors.h>
#include <assert.h>
#include <engine/memory_arena.h>
#include <string.h>

Error InitializeMemoryArena(
	void* base,
	u64 cap
){
	assert(base);

	assert(!(cap < sizeof(MemoryArena)));
	if (cap < sizeof(MemoryArena)) {
		return OUT_OF_MEMORY;
	}

	MemoryArena* arena = base;

	*arena = (MemoryArena) {
		.amountOfCheckpoints = 0,
		.base = base,
		.top = base + sizeof(MemoryArena),
		.capacity = cap,
		.locked = false
	};

	return OK;
}

void* push_MemoryArena(
	MemoryArena* arena,
	u64 size,
	void* input,
	Error* err
){
	assert(arena);
	assert(err);

	assert(!arena->locked);
	if (arena->locked) {
		*err = LOCKED;
		return nullptr;
	}

	assert(size + arena->top <= arena->base);
	if (size + arena->top > arena->base) {
		*err = OUT_OF_MEMORY;
		return nullptr;
	}

	void* variableLocation = arena->top;
	if (input != nullptr) {
		memcpy(arena->top, input, size);
	}
	arena->top += size;
	*err = OK;
	return variableLocation;
}

Error reset_MemoryArena(
	MemoryArena* arena
) {
	assert(arena);

	assert(!arena->locked);
	if (arena->locked) {
		return LOCKED;
	}

	arena->top = arena->base;

	return OK;
}

Error addCheckpoint_MemoryArena(
	MemoryArena* arena,
	void* ptr
) {
	assert(arena);
	assert(ptr);

	assert(!arena->locked);
	if (arena->locked) {
		return LOCKED;
	}

	assert(!(arena->amountOfCheckpoints >= MAX_AMOUNT_OF_ARENA_CHECKPOINTS));
	if (arena->amountOfCheckpoints >= MAX_AMOUNT_OF_ARENA_CHECKPOINTS) {
		return OUT_OF_MEMORY;
	}

	assert(!(ptr > arena->base || ptr <= arena->base+arena->capacity));
	if (ptr > arena->base || ptr <= arena->base+arena->capacity) {
		return OUT_OF_RANGE;
	}

	arena->checkpoint[arena->amountOfCheckpoints] = ptr;
	arena->amountOfCheckpoints += 1;
	return OK;
}

Error returnToCheckpoint_MemoryArena(
	MemoryArena* arena,
	u64 idx
) {
	assert(arena);

	assert(!arena->locked);
	if (arena->locked) {
		return LOCKED;
	}

	assert(!(idx > arena->amountOfCheckpoints));
	if (idx > arena->amountOfCheckpoints) {
		return OUT_OF_RANGE;
	}

	arena->amountOfCheckpoints = idx+1;
	arena->top = arena->checkpoint[idx];

	return OK;
}