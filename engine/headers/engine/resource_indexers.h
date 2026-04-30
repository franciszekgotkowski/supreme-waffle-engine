#pragma once

#include <common/memory_arena.h>
#include <common/typedefs.h>
#include <common/errors.h>
#define MAX_AMOUNT_OF_RESOURCES_IN_INDEXER 100000


// ResourceIndexer is meant to be used strictly in combination with memory arena allocator inside sceneData
// maxAmountOfResources is amount of indexes that this indexer can manage. It has to be lower than MAX_AMOUNT_OF_RESOURCES_IN_INDEXER
// Variabel 'indexes' points to memory address after corresponding ResourceIndexer struct - it is basically pointer to make size of array not hard coded
typedef struct {
	u32 maxAmountOfResources;
	u32 currentAmountOfResources;
	void** indexes;
	MemoryArena* arena;
} ResourceIndexer;

#define GetWholeIndexerSize(AmountOfIndexes) \
	(sizeof(ResourceIndexer) + AmountOfIndexes*sizeof(void*))

// Intializes ResourceIndexer on prt address
// It does not return ResourceIndexer because ResourceIndexer lays array of void pointers to its resources right after the struct;
void InitializeResourceIndexer(
	void* ptr,
	u32 maxAmountOfResources,
	MemoryArena* arena
);

// Initializes ResourceIndexer same as on InitializeResourceIndexer
// but it automatically registers space in arena for iteself
// Can return errors
//	- OK 				if everything went well
//	- OUT_OF_MEMORY		if there is no memory in arena to accomodate this indexer
//	- LOCKED			if memory arena is locked
ResourceIndexer* InitializeResourceIntoArena(
	MemoryArena* arena,
	u32 maxAmountOfResources,
	Error* err
);

// Will register a new resource if it has capacity and will return its ID
// If there will be any error ResourceID it returns will be -1
// Can return in error:
//	- OK
//	- LOCKED				if memory arena is locked
//	- OUT_OF_INDEXES		if there is no more indexes to fill in
//	- OUT_OF_MEMORY			if there is no memory left in the arena
ID RegisterNewResource_ResourceIndexer(
	ResourceIndexer* indexer,
	u64 size,
	Error* err
);

// Gets resource under ID
// Can return in error:
//	- OK
// 	- OUT_OF_RANGE			if index is below 0 or if there is no resource under given ID
void* GetResource_ResourceIndexer(
	ResourceIndexer* indexer,
	ID id,
	Error* err
);