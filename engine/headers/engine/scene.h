#pragma once

#include <engine/errors.h>
#include <engine/memory_pool.h>

#include <engine/ui.h>
#include <engine/area.h>
#include <engine/typedefs.h>
#include <engine/platform/memory_allocations.h>

// MEMORY REGION DEFINES
#define SCENE_FILES_LOCATION "../../assets/scene/"
#define SIZEOF_XS_SCENE (5*MB)
#define SIZEOF_S_SCENE (25*MB)
#define SIZEOF_M_SCENE (75*MB)
#define SIZEOF_L_SCENE (150*MB)
#define SIZEOF_XL_SCENE (300*MB)

#define SIZEOF_LARGEST_SCENE SIZEOF_XL_SCENE
#define SIZEOF_LOADING_SCREEN_SCENE SIZEOF_S_SCENE

// Assets are meant for storing asset data like textures, meshes, font data etc.
// They are not meant to store any data about objects in current scene
typedef struct {
	u64 size;
	void* ptr;
} Asset;

// GameObjects store info about instances of game objects in the scene.
// For example they don't store info about how a game entity looks but store where it is, what animation state it is etc.
typedef struct {
	u64 size;
	void* ptr;
} GameObject;

#define MAXIMUM_AMOUNT_OF_ASSETS_IN_SCENE 100
#define MAXIMUM_AMOUNT_OF_GAME_OBJECTS_IN_SCENE 1000
typedef struct {
	u64 capacity;
	void* data; // pointer to where data storage starts (should be address after address of this struct)
	void* stackTop;
	u64 amountOfAssets;
	u64 amountOfGameObjects;
	Asset asset[MAXIMUM_AMOUNT_OF_ASSETS_IN_SCENE];
	GameObject gameObject[MAXIMUM_AMOUNT_OF_GAME_OBJECTS_IN_SCENE];
} SceneData ;

typedef u64 AssetID;
typedef u64 GameObjectID;

// There are 2 types of files: .scene and .ui.
// They contain elements that will be loaded into scene
// First contents of .ui are loaded and then .scene
// There is separation to make it easy to load in the same ui into 2 scenes
Error InitializeScene(
	SceneData* sceneData, // where function should start initializing data
	u64 size,
	str uiPath,
	str areaPath
);

Error LoadGameScene(
	str uiPath,
	str areaPath
);

Error LoadLoadingScreenScene(
	str uiPath,
	str areaPath
);

// Function will reserve space for asset if it is possible.
// Function will return index of asset placed
// It can return in err:
// 	- OK 					if everything went fine
// 	- OUT_OF_MEMORY			if there was to little memory
// 	- OUT_OF_INDEXES		if there was no index left
AssetID RegisterNewAsset(
	SceneData* sceneData,
	u64 size,
	Error* err
);

// Function will reserve space for gameObject if it is possible.
// Function will return index of gameObject placed
// It can return in err:
// 	- OK 					if everything went fine
// 	- OUT_OF_MEMORY			if there was to little memory
// 	- OUT_OF_INDEXES		if there was no index left
GameObjectID RegisterNewGameObject(
	SceneData* sceneData,
	u64 size,
	Error* err
);