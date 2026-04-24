#pragma once

#include <engine/static_resources.h>
#include <common/errors.h>
#include <common/typedefs.h>
#include <stdbool.h>

#include <platform/memory_allocations.h>

// MEMORY REGION DEFINES
#define SCENE_FILES_LOCATION "../../assets/scene/"
#define SIZEOF_XS_SCENE (5*MB)
#define SIZEOF_S_SCENE (25*MB)
#define SIZEOF_M_SCENE (75*MB)
#define SIZEOF_L_SCENE (150*MB)
#define SIZEOF_XL_SCENE (300*MB)

#define SIZEOF_LARGEST_SCENE SIZEOF_XL_SCENE
#define SIZEOF_LOADING_SCREEN_SCENE SIZEOF_S_SCENE

// Each scene has its own static resources.
// Static resources are basically any resources that are loaded in at scene startup.
// They are not assets for the game, they can be for example: TextRenderingObject or ResourceIndexer's
// Resource indexers are a way to manage resources. They serve a way to allocate, read/write data for resources and to modify them. For example all NPC resources could have update position function

typedef struct {
	u64 maximumCapacity;
	void* data; // pointer to where data storage starts (should be address after address of this struct)
	void* stackTop;
	StaticResourcesIndexer staticResourcesIndexer;
} SceneData ;

// There are 2 types of files: .scene and .ui.
// They contain elements that will be loaded into scene
// First contents of .ui are loaded and then .scene
// There is separation to make it easy to load in the same ui into 2 scenes
Error InitializeScene(
	SceneData* sceneData, // where function should start initializing data
	u64 size,
	str uiPath,
	str areaPath,
	bool exist[AMOUNT_OF_STATIC_RESOURCES]
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
void* PushNewResource(
	SceneData* sceneData,
	u64 size,
	Error* err
);

// Gets a StaticResourceIndexer pointer from scene
// error can be:
//	- OK
//	- DOES_NOT_EXIST
StaticResourcesIndexer* GetStaticResourceIndexer(
	SceneData* sceneData,
	StaticResources staticResources,
	Error* err
);