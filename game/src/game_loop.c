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

void GameLoop() {
    const PointerTable *table = GameMemory;
    assert(table);

    FileData file;
    readEntireTextFile("../../assets/fonts/bdf/cherry-11-b.bdf", &file);
    Font f = InitializeFont(file);
    llu fontSize = GetSizeForEntireFont(&f);
    printf("space needed for cherry-11-r.bdf = %llu\n", fontSize);
    Font *font = alloca(fontSize);
    *font = f;
    InitializeCharacterDataOntoFont(font, file);
    freeEntireTextFile(file);
    stbi_write_png("./cherryfont.png", (i32) bitmapW(font), (i32) bitmapH(font), 1, font->characterBitmap,
                   (i32) bitmapW(font));

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

    // TODO: i have to somehow assert that i cannot use non ascii characters
    str s = "Jak Kuba Bogu tak Bog Kubie...";
    u32 bufsize = strlen(s) * 4 * 4 * sizeof(f32);
    printf("bufsize = %d\n", bufsize);
    f32* buf = alloca(bufsize);

    u32 ebobufsize = strlen(s) * sizeof(u32) * 6;
    u32* ebobuf = alloca(ebobufsize);

    InitializeTextureCoordinatesBuffer(s, strlen(s), 3, font, buf, ebobuf);

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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void *) (2 * sizeof(f32)));


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
    TimeStamp since = PrintTimeSince(windowData->bootTimeStamp);
    f64 fsec = (f64)since.sec;
    f64 fnsectosec = (f64)since.nsec/1000000000.f;
    f64 fsince = fsec + fnsectosec;
    f64 fps = (f64)frames/fsince;
    printf("Amount of frames: %llu\n", (llu)frames);
    printf("Average fps: %f\n", fps);
    CloseWindow();
}
