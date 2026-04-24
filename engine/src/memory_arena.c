#include <common/typedefs.h>
#include <engine/memory_pool.h>
#include <common/errors.h>
#include <assert.h>
#include <engine/memory_arena.h>
#include <string.h>

Error InitializeMemoryArena(
	void* base,
	u64 cap
){
	assert(base);

	// assert(!(cap < sizeof(MemoryArena)));
	if (cap < sizeof(MemoryArena)) {
		return OUT_OF_MEMORY;
	}

	MemoryArena* arena = base;

	*arena = (MemoryArena) {
		.amountOfCheckpoints = 0,
		.base = base + sizeof(MemoryArena),
		.top = base + sizeof(MemoryArena),
		.capacity = cap - sizeof(MemoryArena),
		.locked = false
	};

	return OK;
}

void* registerMemory_MemoryArena(
	MemoryArena* arena,
	u64 size,
	Error* err
){
	assert(arena);
		assert(err);

		if (arena->locked) {
			*err = LOCKED;
			return nullptr;
		}

		if (size + arena->top > arena->base) {
			*err = OUT_OF_MEMORY;
			return nullptr;
		}

		void* ptr = arena->top;
		arena->top += size;

		*err = OK;
		return ptr;
}

void* registerAndInitializeMemory_MemoryArena(
	MemoryArena* arena,
	u64 size,
	void* input,
	Error* err
){
	assert(arena);
	assert(err);

	void* t = registerMemory_MemoryArena(
		arena,
		size,
		err
	);

	if ( *err != OK ) {
		return NULL;
	}

	memcpy(
		t,
		input,
		size
	);
	return t;

}

Error reset_MemoryArena(
	MemoryArena* arena
) {
	assert(arena);

	if (arena->locked) {
		return LOCKED;
	}

	arena->top = arena->base;
	arena->amountOfCheckpoints = 0;

	return OK;
}

CheckpointID addCheckpoint_MemoryArena(
	MemoryArena* arena,
	void* ptr,
	Error* err
) {
	assert(arena);
	assert(ptr);

	if (arena->locked) {
		*err =  LOCKED;
		return 0;
	}

	if (arena->amountOfCheckpoints >= MAX_AMOUNT_OF_ARENA_CHECKPOINTS) {
		*err =  OUT_OF_INDEXES;
		return 0;
	}

	if (ptr < arena->base || ptr > arena->base+arena->capacity) {
		*err =  OUT_OF_RANGE;
		return 0;
	}

	if (arena->amountOfCheckpoints > 0) {
		if (ptr < arena->checkpoint[arena->amountOfCheckpoints-1]) {
			*err = INVALID_INPUT;
			return 0;
		}
	}

	arena->checkpoint[arena->amountOfCheckpoints] = ptr;
	arena->amountOfCheckpoints += 1;
	*err =  OK;
	return arena->amountOfCheckpoints - 1;
}

Error resetToCheckpoint_MemoryArena(
	MemoryArena* arena,
	CheckpointID id
) {
	assert(arena);

	if (arena->locked) {
		return LOCKED;
	}

	if (id > arena->amountOfCheckpoints) {
		return OUT_OF_RANGE;
	}

	arena->amountOfCheckpoints = id+1;
	arena->top = arena->checkpoint[id];

	return OK;
}