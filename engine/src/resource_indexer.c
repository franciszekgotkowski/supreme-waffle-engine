#include <assert.h>
#include <common/typedefs.h>
#include <engine/resource_indexers.h>

void InitializeResourceIndexer(
	void* ptr,
	u32 maxAmountOfResources
) {
	assert(ptr);
	assert(maxAmountOfResources < MAX_AMOUNT_OF_RESOURCES_IN_INDEXER);

	ResourceIndexer* riPtr = ptr;
	*riPtr = (ResourceIndexer){
		.maxAmountOfResources = maxAmountOfResources,
		.currentAmountOfResources = 0,
		.indexes = ptr + sizeof(ResourceIndexer)
	};
}