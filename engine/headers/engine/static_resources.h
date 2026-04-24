#pragma once

#include <engine/memory_pool.h>
#include <engine/resource_indexers.h>
#include <engine/text_rendering.h>
#include <stdbool.h>

typedef enum {
	TEXT_RENDERING_OBJECT,
	FONT_INDEXER,
	OBJECT_3D_INDEXER,

	AMOUNT_OF_STATIC_RESOURCES
} StaticResources;

// Indexer made specifically to contain Indexers that will be dynamically allocated on the scene stack if needed. It lives in SceneData struct
typedef struct {
	bool exist[AMOUNT_OF_STATIC_RESOURCES];
	void* ptr[AMOUNT_OF_STATIC_RESOURCES];
} StaticResourcesIndexer;

// Returns pointer to requested StaticResource.
// Errors it can return:
// 	- OK
// 	- DOES_NOT_EXIST		in this case funcion will also return NULL
void* GetStaticResource(
	StaticResourcesIndexer* staticSceneResources,
	StaticResources resource,
	Error* err
);