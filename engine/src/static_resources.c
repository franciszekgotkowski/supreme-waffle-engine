#include <engine/text_rendering.h>
#include <common/errors.h>
#include <assert.h>
#include <engine/static_resources.h>
#include <engine/resource_indexers.h>
#include <common/typedefs.h>
#include <string.h>
#include <stddef.h>

extern const u32 DefaultAmountsOfIndexes[AMOUNT_OF_STATIC_RESOURCES];

void* GetStaticResource_StaticResourceIndexer(
	StaticResourcesIndexer* staticSceneResources,
	StaticResources resource,
	Error* err
) {
	assert(staticSceneResources);
	assert(resource < AMOUNT_OF_STATIC_RESOURCES);
	assert(err);

	if (!staticSceneResources->exist[resource]) {
		*err = DOES_NOT_EXIST;
		return NULL;
	}

	*err = OK;
	return staticSceneResources->ptr[resource];
}

// This is function that will select correct initalizer based on enum of static resource
void InitializeStaticResource(
	void* ptr, //where to initialize
	StaticResources staticResources,
	MemoryArena* arena
) {
	assert(ptr);
	assert(staticResources < AMOUNT_OF_STATIC_RESOURCES);
	assert(arena);

	switch (staticResources) {
		case TEXT_RENDERING_OBJECT:
			InitializeTextRenderingObject(
				ptr
			);
			break;
		case FONT_INDEXER:
			InitializeResourceIndexer(
				ptr,
				DefaultAmountsOfIndexes[FONT_INDEXER],
				arena
			);
			break;
		case OBJECT_3D_INDEXER:
			InitializeResourceIndexer(
				ptr,
				DefaultAmountsOfIndexes[OBJECT_3D_INDEXER],
				arena
			);
			break;
		case SHADER_PROGRAM_INDEXER:
			InitializeResourceIndexer(
				ptr,
				DefaultAmountsOfIndexes[SHADER_PROGRAM_INDEXER],
				arena
			);
			break;
		default:
			assert(false);
			break;
	}
}
