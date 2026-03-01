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

    err = LoadGameScene(
    	"stub",
     	"stub"
    );
    assert(err == OK);

    err = LoadLoadingScreenScene(
     	"stub",
      	"stub"
    );
    assert(err == OK);

    // just experimenting with loading fonts into memory
    // to be moved elsewhere
  //   {
  //  		FileData file;
  //    	readEntireTextFile("../../assets/fonts/bdf/cherry-11-r.bdf", &file);
  //     	Font f = InitializeFont(file);
  //      	printf("space needed for cherry-11-r.bdf = %llu\n", (llu)GetSizeForEntireFont(&f));

  //       Font* font = alloca(GetSizeForEntireFont(&f));


		// *font = f;
  //       InitializeCharacterDataOntoFont(font, file);

  //       freeEntireTextFile(file);

  //       stbi_write_png("./cherryfont.png", (i32)bitmapW(font), (i32)bitmapH(font), 1, font->characterBitmap, (i32)bitmapW(font));
  //       // CreateRenderObject
  //       // DrawRednerObject
  //   }

    GameLoop();

	printf("amount of memory regions in enum %d\n", AMOUNT_OF_ENGINE_MEMORY_REGIONS);
	// InitializeGameState()
	// LoadSceneFromDisk();
}
