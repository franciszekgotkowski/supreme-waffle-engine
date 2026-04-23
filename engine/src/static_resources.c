#include <common/errors.h>
#include <assert.h>
#include <engine/static_resources.h>
#include <common/typedefs.h>
#include <string.h>
#include <stddef.h>

void* GetStaticResource(
	StaticResourcesIndexer* staticSceneResources,
	StaticResources resource,
	Error* err
) {
	assert(staticSceneResources);
	assert(resource < AMOUNT_OF_STATIC_SCENE_RESOURCES);
	assert(err);

	if (!staticSceneResources->exist[resource]) {
		*err = DOES_NOT_EXIST;
		return NULL;
	}

	return staticSceneResources->ptr[resource];
}