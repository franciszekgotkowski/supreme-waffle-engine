#include <platform/measure_time.h>
#include <platform/shader.h>
#include <platform/graphics.h>
#include <game/input_functions.h>
#include <engine/memory_pool.h>
#include <engine/window.h>
#include <game/game_loop.h>
#include <engine/game_loop.h>

#include <platform/file_io.h>
#include <engine/font.h>
#include <engine/scene.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <engine/text_rendering.h>
#include <common/errors.h>
#include <platform/window.h>

#include <math.h>
#include <glfw3.h>

extern PointerTable *GameMemory;
Error err;

void GameLoop() {
	const PointerTable *table = GameMemory;
	assert(table);


// TEMP CODE
	FileData file;
	readEntireTextFile("../../assets/fonts/bdf/cherry-11-b.bdf", &file);
	// readEntireTextFile("../../assets/fonts/bdf/t0-16.bdf", &file);

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
	// printf("Font size in bytes: %llu\n", (llu)GetSizeForEntireFont(font));


	GameObjectID TextID = RegisterNewGameObject(
		sceneData,
		MAX_SIZE_FOR_CHARACTER_RENDER_DATA,
		&err
	);
	assert(err == OK);

	err = InitializeTextRenderingObject(
		TextID,
		sceneData
	);
	assert(err == OK);

	TextData* textData = sceneData->gameObject[TextID].ptr;

	str s = "......Jak Kuba Bogu tak Bog Kubie...";
	err = AppendNewLine(
		s,
		strlen(s),
		sceneData->gameObject[TextID].ptr,
		sceneData->asset[CherryFontID].ptr,
		(v2){
			.x = 0.0f,
			.y = 0.0f,
		},
		(Color){
			.r = 1.0f,
			.g = 0.2f,
			.b = 0.5f,
			.a = 1.0f
		},
		3
	);
	assert(err == OK);

	str s2 = "o-o-o-o";
	err = AppendNewLine(
		s2,
		strlen(s2),
		sceneData->gameObject[TextID].ptr,
		sceneData->asset[CherryFontID].ptr,
		(v2){
			.x = 1.0f,
			.y = -1.0f,
		},
		(Color){
			.r = 1.0f,
			.g = 0.7f,
			.b = 0.1f,
			.a = 1.0f
		},
		8
	);
	assert(err == OK);

	str s3 = "Erysipelothrix!!!";
	err = AppendNewLine(
		s3,
		strlen(s3),
		sceneData->gameObject[TextID].ptr,
		sceneData->asset[CherryFontID].ptr,
		(v2){
			.x = 1.0f,
			.y = 0.0f,
		},
		(Color){
			.r = 1.0f,
			.g = 0.0f,
			.b = 1.0f,
			.a = 1.0f
		},
		2
	);
	assert(err == OK);



	ShaderProgramID TextShader = CreateShaderProgram("../../engine/src/shaders/render_text.vert", "../../engine/src/shaders/render_text.frag");
	// ShaderProgramID TextShader = CreateShaderProgram("../../engine/src/shaders/font.vert", "../../engine/src/shaders/font.frag");

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
	glBufferData(GL_ARRAY_BUFFER, SpaceNeededForVBO(textData->amountOfCharacters), textData->verticiesStart, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, SpaceNeededForEBO(textData->amountOfCharacters), textData->indiciesStart, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void *) 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void *) (2 * sizeof(f32)));
	glEnableVertexAttribArray(2);
	glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, 5 * sizeof(f32), (void *) (4 * sizeof(f32)));


	UseShaderProgram(TextShader);

	glBindTexture(GL_TEXTURE_2D, textureID);

	// GLint TextureLocation = glGetUniformLocation(TextShader, "FontTexture");
	// GLint ColorLocation = glGetUniformLocation(TextShader, "fontColor");
	// GLint OffsetLocation = glGetUniformLocation(TextShader, "offset");

	GLint uOffset = glGetUniformLocation(TextShader, "uOffset");
	GLint uScale = glGetUniformLocation(TextShader, "uScale");
	GLint uColor = glGetUniformLocation(TextShader, "uColor");
	GLint uShouldDraw = glGetUniformLocation(TextShader, "uShouldDraw");
	GLint uFontTexture = glGetUniformLocation(TextShader, "uFontTexture");

	glBindVertexArray(VAO);
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

		// textData->line[0].offset.y = -1.0f-sin(glfwGetTime());
		textData->linesData.offset[0].y = -1.0f-sin(glfwGetTime());
		textData->linesData.color[0].g = 0.5f+0.5f*sin(10*glfwGetTime());

		// glUniform2fv(OffsetLocation, 1, textData->linesData.offset[0].arr);
		// glUniform3fv(ColorLocation, 1, textData->linesData.color[0].rgb);
		// glUniform1i(TextureLocation, 0);
		// glUniform3fv(TextureLocation, 1, textData->linesData.color[0].rgb);

		glUniform2fv(uOffset, MAX_AMOUNT_OF_LINES_IN_SCENE, (const GLfloat*)(textData->linesData.offset));
		glUniform1uiv(uScale, MAX_AMOUNT_OF_LINES_IN_SCENE, (const GLuint*)(textData->linesData.scale));
		glUniform4fv(uColor, MAX_AMOUNT_OF_LINES_IN_SCENE, (const GLfloat*)(textData->linesData.color));
		glUniform1iv(uShouldDraw, MAX_AMOUNT_OF_LINES_IN_SCENE/4, (const GLint*)(textData->linesData.shouldDraw));
		glUniform1i(uFontTexture, 0);

		// glUniform2fv(loc, 1, offset);
		u32 m = textData->amountOfCharacters;
		glDrawElements(GL_TRIANGLES, m*6, GL_UNSIGNED_INT, (void *) 0);

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
