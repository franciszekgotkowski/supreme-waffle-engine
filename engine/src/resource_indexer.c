#include <common/errors.h>
#include <common/memory_arena.h>
#include <assert.h>
#include <stddef.h>
#include <common/typedefs.h>
#include <engine/resource_indexers.h>
#include <strings.h>

void InitializeResourceIndexer(
	void* ptr,
	u32 maxAmountOfResources,
	MemoryArena* arena
) {
	assert(ptr);
	assert(maxAmountOfResources < MAX_AMOUNT_OF_RESOURCES_IN_INDEXER);
	assert(arena);

	ResourceIndexer* riPtr = ptr;
	*riPtr = (ResourceIndexer){
		.maxAmountOfResources = maxAmountOfResources,
		.currentAmountOfResources = 0,
		.indexes = ptr + sizeof(ResourceIndexer),
		.arena = arena
	};
}

ResourceIndexer* InitializeResourceIntoArena(
	MemoryArena* arena,
	u32 maxAmountOfResources,
	Error* err
) {
	assert(arena);
	assert(maxAmountOfResources < MAX_AMOUNT_OF_RESOURCES_IN_INDEXER);

	Error e;
	void* memory = registerMemory_MemoryArena(
		arena,
		GetWholeIndexerSize(maxAmountOfResources),
		&e
	);

	if (e != OK) {
		*err = e;
		return NULL;
	} else {
		*err = OK;
	}

	InitializeResourceIndexer(
		memory,
		maxAmountOfResources,
		arena
	);

	return memory;
}

ID RegisterNewResource_ResourceIndexer(
	ResourceIndexer* indexer,
	u64 size,
	Error* err
) {
	assert(indexer);
	assert(err);

	if (indexer->arena->locked) {
		*err = LOCKED;
		return -1;
	}

	if (indexer->currentAmountOfResources >= indexer->maxAmountOfResources) {
		*err = OUT_OF_INDEXES;
		return -1;
	}

	Error error;
	void* ptr = registerMemory_MemoryArena(
		indexer->arena,
		size,
		&error
	);

	if (error != OK) {
		assert(error == OUT_OF_MEMORY || error == LOCKED);
		if (error == OUT_OF_MEMORY) {
			*err = OUT_OF_MEMORY;
		} else {
			*err = LOCKED;
		}
		return -1;
	}

	indexer->indexes[indexer->currentAmountOfResources] = ptr;
	indexer->currentAmountOfResources++;

	*err = OK;
	return indexer->currentAmountOfResources-1;
}

void* GetResource_ResourceIndexer(
	ResourceIndexer* indexer,
	ID id,
	Error* err
) {
	assert(indexer);
	assert(err);
	// assert(id < indexer->currentAmountOfResources);

	if (
		id >= indexer->currentAmountOfResources ||
		id < 0
	) {
		*err = OUT_OF_RANGE;
		return NULL;
	}

	*err = OK;
	return indexer->indexes[id];
}
