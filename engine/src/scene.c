#include <engine/errors.h>
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
	assert(0);
	return LOGIC_ERROR;
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