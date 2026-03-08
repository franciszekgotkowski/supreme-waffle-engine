#pragma once

#include <engine/memory_pool.h>

#include <engine/ui.h>
#include <engine/area.h>
#include <engine/typedefs.h>
#include <engine/platform/memory_allocations.h>

#define SCENE_FILES_LOCATION "../../assets/scene/"
#define SIZEOF_XS_SCENE (5*MB)
#define SIZEOF_S_SCENE (25*MB)
#define SIZEOF_M_SCENE (75*MB)
#define SIZEOF_L_SCENE (150*MB)
#define SIZEOF_XL_SCENE (300*MB)

#define SIZEOF_LARGEST_SCENE SIZEOF_XL_SCENE
#define SIZEOF_LOADING_SCREEN_SCENE SIZEOF_S_SCENE

#define MAX_AMOUNT_OF_FONTS_PER_SCENE 10
#define SIZEOF_MEMORY_FOR_FONTS (MAX_AMOUNT_OF_FONTS_PER_SCENE*(KB*50))

#define MAX_AMOUNT_OF_CHARS_PER_SCENE (10*KB)
#define MAX_AMOUNT_OF_LINES_IN_SCENE (1*KB)

#define MAX_SIZE_FOR_TEXT (MAX_AMOUNT_OF_CHARS_PER_SCENE*sizeof(char))
#define MAX_SIZE_FOR_LINE_OFFSETS (MAX_AMOUNT_OF_LINES_IN_SCENE*sizeof(u32)*2)
#define MAX_SIZE_FOR_LINE_COLORS (MAX_AMOUNT_OF_LINES_IN_SCENE*sizeof(u8)*4)
#define MAX_SIZE_FOR_LINE_SIZES (MAX_AMOUNT_OF_LINES_IN_SCENE*sizeof(u32))
// ilość charów *
// floaty dla xy dla koordynatów na ekranie + koordynatów tekstury
// + indeksy EBO
#define MAX_SIZE_FOR_CHARACTER_RENDER_DATA (MAX_AMOUNT_OF_CHARS_PER_SCENE*\
	(sizeof(f32)*2*4*2+\
	sizeof(u32)*6))

#define TOTAL_SIZE_FOR_TEXT_RENDERING (\
	SIZEOF_MEMORY_FOR_FONTS+\
	MAX_SIZE_FOR_TEXT+\
	MAX_SIZE_FOR_LINE_OFFSETS+\
	MAX_SIZE_FOR_LINE_COLORS+\
	MAX_SIZE_FOR_LINE_SIZES+\
	MAX_SIZE_FOR_CHARACTER_RENDER_DATA)

typedef struct {
	u64 sceneCapacity;
	void* data;
} SceneData ;

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