#include <../headers/engine/handle_input.h>
#include <common/errors.h>
#include <assert.h>
#include <engine/memory_pool.h>
#include <engine/scene.h>
#include <string.h>
#include <common/range.h>
#include <engine/static_resources.h>

extern PointerTable* GameMemory;

extern const u32 SizesForEachStaticResource[AMOUNT_OF_STATIC_SCENE_RESOURCES];

static void InitializeStaticResourceIndexer(
	SceneData* sceneData,
	bool exist[AMOUNT_OF_STATIC_SCENE_RESOURCES]
) {
	assert(sceneData);
	assert(exist);

	memcpy(
		&(sceneData->staticResourcesIndexer.exist),
		&exist,
		(AMOUNT_OF_STATIC_SCENE_RESOURCES * sizeof(bool))
	);

	Error err;
	for range(i, AMOUNT_OF_STATIC_SCENE_RESOURCES) {
		if (sceneData->staticResourcesIndexer.exist[i]) {
			assert(SizesForEachStaticResource[i] != 0);
			void* t = PushNewResource(
				sceneData,
				SizesForEachStaticResource[i],
				&err
			);
			assert(err == OK);
			assert(t);
			sceneData->staticResourcesIndexer.ptr[i] = t;
			sceneData->staticResourcesIndexer.exist[i] = true;
		}
	}
}

Error InitializeScene(
	SceneData* sceneData,
	u64 size,
	str uiPath,
	str areaPath,
	bool StaticResourcesExist[AMOUNT_OF_STATIC_SCENE_RESOURCES]
) {
	assert(sceneData);
	assert(uiPath);
	assert(areaPath);
	assert(sizeof(SceneData) <= size);

	*sceneData = (SceneData){
		.maximumCapacity = size - sizeof(SceneData),
		.data = (void*)(sceneData + 1),
		.stackTop = (void*)(sceneData + 1),
		.staticResourcesIndexer = {}
	};

	InitializeStaticResourceIndexer(
		sceneData,
		StaticResourcesExist
	);

	// TODO
	// loadUi()
	// loadArea()

	return OK;
}

// Last parameter is temporary!!!
// It will be autodetermined by the scene fiele when the time is right
Error LoadGameScene(
	str uiPath,
	str areaPath
) {
	assert(uiPath);
	assert(areaPath);

	return InitializeScene(
		getRegion(GAME_SCENE),
		getRegionCapacity(GameMemory, GAME_SCENE),
		uiPath,
		areaPath,
		(bool[]){
			true,
			true,
			true
		}
	);

	return OK;
}

// Last parameter is temporary!!!
// It will be autodetermined by the scene fiele when the time is right
Error LoadLoadingScreenScene(
	str uiPath,
	str areaPath
) {
	assert(uiPath);
	assert(areaPath);

	return InitializeScene(
		getRegion(LOADING_SCREEN_SCENE),
		getRegionCapacity(GameMemory, LOADING_SCREEN_SCENE),
		uiPath,
		areaPath,
		(bool[]){
			true,
			true,
			true
		}
	);

	return OK;
}

void* PushNewResource(
	SceneData *sceneData,
	u64 size,
	Error *err
) {
	assert(sceneData);
	assert(err);

	if ( sceneData->stackTop + size > sceneData->data + sceneData ->maximumCapacity) {
		*err = OUT_OF_MEMORY;
		return NULL;
	}

	void* oldTop = sceneData->stackTop;
	sceneData->stackTop += size;
	*err = OK;
	return oldTop;
}