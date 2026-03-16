#include <engine/platform/measure_time.h>
#include <engine/platform/shader.h>
#include <external/glad/glad.h>
#include <engine/platform/graphics.h>
#include <game/input_functions.h>
#include <engine/memory_pool.h>
#include <engine/platform/window_data.h>
#include <engine/platform/input.h>
#include <game/game_loop.h>
#include <engine/platform/game_loop.h>

#include <external/stb_image_write.h>

#include <engine/platform/file_io.h>
#include <engine/platform/crossplatform_alloca.h>
#include <engine/font.h>
#include <engine/scene.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <math.h>
#include <external/glfw3.h>

extern PointerTable *GameMemory;
Error err;

void GameLoop() {
	const PointerTable *table = GameMemory;
	assert(table);


// TEMP CODE
	FileData file;
	readEntireTextFile("../../assets/fonts/bdf/cherry-11-b.bdf", &file);

	SceneData* sceneData = (SceneData*)getRegion(LOADING_SCREEN_SCENE);
	AssetID CherryFontID = RegisterNewAsset(
		sceneData,
		SIZEOF_MEMORY_FOR_FONTS,
		&err
	);
	assert(err == OK);

	Font *font = sceneData->asset[CherryFontID].ptr;
	*font =  InitializeFont(file);
	InitializeCharacterDataOntoFont(font, file);
	freeEntireTextFile(file);



	GameObjectID gameObjectID = RegisterNewGameObject(
		sceneData,
		MAX_SIZE_FOR_CHARACTER_RENDER_DATA,
		&err
	);
	assert(err == OK);

	str s = "Jak Kuba Bogu tak Bog Kubie...";
	void* ptr = sceneData->gameObject[gameObjectID].ptr;
	strcpy(ptr, s);
	ptr += sizeof(char)*(strlen(ptr)+1);

	f32* buf = ptr;
	u32 bufsize = (strlen(s)+1) * 5 * 4 * sizeof(f32);
	ptr += bufsize;

	u32* ebobuf = ptr;
	u32 ebobufsize = (strlen(s)+1) * sizeof(u32) * 6;
	ptr += ebobufsize;

	InitializeTextureCoordinatesBuffer(s, strlen(s), 3, font, buf, ebobuf);
// END OF TEMP CODE





	ShaderProgramID FontShader = CreateShaderProgram("../../engine/src/shaders/font.vert",
		"../../engine/src/shaders/font.frag");

	u32 textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, (i32) bitmapW(font), (i32) bitmapH(font), 0, GL_RED, GL_UNSIGNED_BYTE,
		font->characterBitmap);

	glGenerateMipmap(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	u32 VAO;
	u32 VBO;
	u32 EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, bufsize, buf, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebobufsize, ebobuf, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void *) 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void *) (2 * sizeof(f32)));


	UseShaderProgram(FontShader);

	f32 offset[2] = {0.0f, 0.0f};
	glBindTexture(GL_TEXTURE_2D, textureID);
	GLint loc = glGetUniformLocation(FontShader, "FontTexture");
	glUniform1i(loc, 0);
	loc = glGetUniformLocation(FontShader, "fontColor");
	f32 color[3] = {1.0f, 0.5f, 0.2f};
	glUniform3fv(loc, 1, color);
	glBindVertexArray(VAO);


	loc = glGetUniformLocation(FontShader, "offset");
	printf("TOTAL_SIZE_FOR_TEXT_RENDERING = %lluMb %lluKb %llub\n", (llu)TOTAL_SIZE_FOR_TEXT_RENDERING/MB, (llu)((TOTAL_SIZE_FOR_TEXT_RENDERING%MB)/KB), (llu)(TOTAL_SIZE_FOR_TEXT_RENDERING%KB));

	WindowData* windowData = getRegion(WINDOW_DATA);
	windowData->frametimeEveningTimeStamp = InitializeTimeStamp();
	TimeStamp startDrawingStamp = InitializeTimeStamp();
	while (!((WindowData *) getRegion(WINDOW_DATA))->windowShouldClose) {
		clearScreen();

		handleInput();
		// handleEditor();
		// handleGameEvents(table);
		// renderScene(table);

		offset[1] = -1.0f-sin(glfwGetTime());
		glUniform2fv(loc, 1, offset);
		glDrawElements(GL_TRIANGLES, strlen(s)*6, GL_UNSIGNED_INT, (void *) 0);

		updateBuffer();
		handleEngineEvents();

		windowData->framesElapsed++;
		MatchFrametime(windowData->frametime, windowData->frametimeEveningTimeStamp);
		windowData->frametimeEveningTimeStamp = InitializeTimeStamp();
	}

	u64 frames = windowData->framesElapsed;
	// TODO: probably some number overflow in PrintTimeSince on miliseconds
	TimeStamp since = PrintTimeSince(startDrawingStamp);
	f64 fsec = (f64)since.sec;
	f64 fnsectosec = (f64)since.nsec/1000000000.f;
	f64 fsince = fsec + fnsectosec;
	f64 fps = (f64)frames/fsince;
	printf("Amount of frames: %llu\n", (llu)frames);
	printf("Average fps: %f\n", fps);
	TimeStamp loadTime = TimeDiff(startDrawingStamp, windowData->bootTimeStamp);
	u64 loadTimeMs = loadTime.sec*1000000000 + loadTime.nsec;
	printf("Initialization time: %llums %lluns\n", (llu)loadTimeMs/1000000, (llu)loadTimeMs%1000000);
	CloseWindow();
}
