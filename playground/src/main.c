#include <assert.h>

#include <common/typedefs.h>
#include <common/crossplatform_alloca.h>
#include <common/errors.h>

#include <engine/scene.h>
#include <engine/memory_pool.h>
#include <engine/handle_input.h>
#include <engine/game_loop.h>
#include <engine/window.h>
#include <game/input_functions.h>
#include <common/cursor_modes.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

PointerTable* GameMemory = NULL;

int main() {
	Error err;

	GameMemory = InitializePool();
	assert(GameMemory);

    InitializeEngineWindow(
     	800,
      	800,
       	60,
        false,
        false,
        (v4){
            .arr = {0.18f, 0.20f, 0.25f, 1.0f}
        },
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
