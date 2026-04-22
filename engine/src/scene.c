// #include "engine/platform/input.h"
#include <../headers/engine/handle_input.h>
#include <common/errors.h>
#include <assert.h>
#include <engine/memory_pool.h>
#include <engine/scene.h>

extern PointerTable* GameMemory;

Error InitializeScene(
	SceneData* sceneData,
	u64 size,
	str uiPath,
	str areaPath
) {
	assert(sceneData);
	assert(uiPath);
	assert(areaPath);
	assert(sizeof(SceneData) <= size);

	*sceneData = (SceneData){
		.maximumCapacity = size - sizeof(SceneData),
		.data = (void*)(sceneData + 1),
		.stackTop = (void*)(sceneData + 1),
		.amountOfAssets = 0,
		.amountOfGameObjects = 0
	};

	// TODO
	// loadUi()
	// loadArea()

	return OK;
}

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
		areaPath
	);

	return OK;
}

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
		areaPath
	);

	return OK;
}

AssetID RegisterNewAsset(
	SceneData *sceneData,
	u64 size,
	Error *err
) {
	assert(sceneData);
	assert(err);

	if (sceneData->amountOfAssets >= MAXIMUM_AMOUNT_OF_ASSETS_IN_SCENE) {
		*err = OUT_OF_INDEXES;
		return 0;
	}

	if (sceneData->stackTop > sceneData->data + sceneData->maximumCapacity) {
		*err = OUT_OF_MEMORY;
		return 0;
	}

	sceneData->asset[sceneData->amountOfAssets] = (Asset){
		.ptr = sceneData->stackTop,
		.size = size
	};

	sceneData->amountOfAssets += 1;
	sceneData->stackTop += size;

	*err = OK;
	return sceneData->amountOfAssets - 1;
}

GameObjectID RegisterNewGameObject(
	SceneData *sceneData,
	u64 size,
	Error *err
) {
	assert(sceneData);
	assert(err);

	if (sceneData->amountOfGameObjects >= MAXIMUM_AMOUNT_OF_GAME_OBJECTS_IN_SCENE) {
		*err = OUT_OF_INDEXES;
		return 0;
	}

	if (sceneData->stackTop > sceneData->data + sceneData->maximumCapacity) {
		*err = OUT_OF_MEMORY;
		return 0;
	}

	sceneData->gameObject[sceneData->amountOfGameObjects] = (GameObject){
		.ptr = sceneData->stackTop,
		.size = size
	};

	sceneData->amountOfGameObjects += 1;
	sceneData->stackTop += size;

	*err = OK;
	return sceneData->amountOfGameObjects - 1;
}