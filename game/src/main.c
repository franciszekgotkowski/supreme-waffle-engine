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

PointerTable* GameMemory = NULL;

int main() {
	// Error err;

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

    // err = LoadLoadingScreenScene(
    //  	"stub",
    //   	"stub"
    // );
    // assert(err == OK);

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
