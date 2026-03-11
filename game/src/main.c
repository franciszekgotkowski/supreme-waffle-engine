#include <assert.h>

#include <engine/scene.h>
#include <engine/typedefs.h>
#include <engine/font.h>
#include <engine/range.h>
#include <engine/memory_arena.h>
#include <engine/platform/file_io.h>
#include <engine/platform/memory_allocations.h>
#include <engine/memory_pool.h>
#include <engine/platform/window_data.h>
#include <engine/platform/input.h>
#include <engine/string_utils.h>
#include <engine/platform/crossplatform_alloca.h>
#include <external/stb_image_write.h>

#include <game/input_functions.h>

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

PointerTable* GameMemory = NULL;

int main() {
	Error err;

	GameMemory = InitializePool();
	assert(GameMemory);

    InitializeWindow(
     	800,
      	800,
       	60,
        false,
        false,
        (v4){.arr = {0.18f, 0.20f, 0.25f, 1.0f}},
        "Der Spiel!",
        CURSOR_NORMAL
    );

    InitializeInputData();
    InitializeInputFunctions();
    InsertInputFunctions();

    err = LoadLoadingScreenScene(
     	"stub",
      	"stub"
    );
    assert(err == OK);


    // DEBUG CODE
   	SceneData* sceneData = (SceneData*)getRegion(LOADING_SCREEN_SCENE);

   	printf("Loading screen scene has: %lluMB %lluKB %lluB\n", (llu)sceneData->capacity/MB, (llu)(sceneData->capacity%MB)/KB, (llu)(sceneData->capacity%KB));
   	printf("Scene struct has: %lluMB %lluKB %lluB\n", (llu)sizeof(SceneData)/MB, (llu)(sizeof(SceneData)%MB)/KB, (llu)(sizeof(SceneData)%KB));
   	u64 togetherSize = sceneData->capacity + sizeof(SceneData);
   	printf("Together they have: %lluMB %lluKB %lluB\n", (llu)togetherSize/MB, (llu)(togetherSize%MB)/KB, (llu)(togetherSize%KB));

   	u64 assetSize = KB/2;
   	u64 asset1 = RegisterNewAsset(
  		sceneData,
  		assetSize,
   	&err
   	);
   	assert(err == OK);
   	memset(sceneData->asset[asset1].ptr, 0, assetSize);


   	u64 gameObjectSize = KB/2;
   	u64 gameObject1 = RegisterNewGameObject(
  		sceneData,
  		gameObjectSize,
   	&err
   	);
   	assert(err == OK);
   	memset(sceneData->gameObject[gameObject1].ptr, 1, gameObjectSize);
    // END OF DEBUG CODE


    // err = LoadGameScene(
    // 	"stub",
    //  	"stub"
    // );
    // assert(err == OK);

    GameLoop();

    printf("amount of memory regions in enum %d\n", AMOUNT_OF_ENGINE_MEMORY_REGIONS);
    // InitializeGameState()
    // LoadSceneFromDisk();
}
