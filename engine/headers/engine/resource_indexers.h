#pragma once

#include <common/typedefs.h>
#define MAX_AMOUNT_OF_RESOURCES_IN_INDEXER 100000

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

// Error InitializeResourceIndexerOntoScene(
// 	u32 maxAmountOfResources,
// )