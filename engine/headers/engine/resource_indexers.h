#pragma once

#include <common/typedefs.h>
#include <common/errors.h>
#define MAX_AMOUNT_OF_RESOURCES_IN_INDEXER 100000

typedef u32 ResourceID;

// ResourceIndexer is meant to be used strictly in combination with SceneStack allocator
// maxAmountOfResources is amount of indexes that this indexer can manage. It has to be lower than MAX_AMOUNT_OF_RESOURCES_IN_INDEXER
// indexes is pointer to place after corresponding ResourceIndexer object - it is basically pointer to make size of array not hard coded
typedef struct {
	u32 maxAmountOfResources;
	u32 currentAmountOfResources;
	void** indexes;
} ResourceIndexer;

#define GetWholeIndexerSize(AmountOfIndexes) \
	(sizeof(ResourceIndexer) + AmountOfIndexes*sizeof(void*))

void InitializeResourceIndexer(
	void* ptr,
	u32 maxAmountOfResources
);

// Will register a new resource if it has capacity and return its ID
// Can return in error:
//	- OK
//	- OUT_OF_RANGE
//	- OUT_OF_MEMORY
ResourceID RegisterNewResource(
	ResourceIndexer* indexer,
	Error* err
);

// Gets resource under ID
void* GetResource(
	ResourceIndexer* indexer
);