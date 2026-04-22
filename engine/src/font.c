#include <engine/memory_pool.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <platform/graphics.h>
#include <platform/file_io.h>
#include <common/string_utils.h>
#include <engine/font.h>
#include <common/typedefs.h>
#include <common/range.h>
#include <platform/memory_allocations.h>
#include <common/crossplatform_alloca.h>

extern PointerTable* GameMemory;

static i32 recogniseFontParameter(str s)
{
    assert(s);
    const u32 amountOfParameters = 6;
    str parameters[] = {
        "FONTBOUNDINGBOX",
        "SPACING",
        "FONT_ASCENT",
        "FONT_DESCENT",
        "ENDPROPERTIES",
        "CHARS",
    };

    for range(i, amountOfParameters)
    {
        if (wordsMatch(parameters[i], s))
        {
            return i;
        }
    }

    return -1;
}

static inline void handle_fontboundingbox(mstr ms, Font* font)
{
    assert(font);
    assert(ms);


    ms = nextWord(ms);
    u64 wordLen = wordLength(ms);
    mstr currentstr = alloca(wordLen+1);
    strncpy(currentstr, ms, wordLen);
    currentstr[wordLen] = '\0';
    font->boundingBoxW = atoi(currentstr);

    ms = nextWord(ms);
    wordLen = wordLength(ms);
    currentstr = alloca(wordLen+1);
    strncpy(currentstr, ms, wordLen);
    currentstr[wordLen] = '\0';
    font->boundingBoxH = atoi(currentstr);

    ms = nextWord(ms);
    wordLen = wordLength(ms);
    currentstr = alloca(wordLen+1);
    strncpy(currentstr, ms, wordLen);
    currentstr[wordLen] = '\0';
    font->offsetX = atoi(currentstr);

    ms = nextWord(ms);
    wordLen = wordLength(ms);
    currentstr = alloca(wordLen+1);
    strncpy(currentstr, ms, wordLen);
    currentstr[wordLen] = '\0';
    font->offsetY = atoi(currentstr);

    return;
}

static inline void handle_spacing(mstr ms, Font* font)
{
    assert(font);
    assert(ms);

    ms = nextWord(ms);
    switch (ms[1])
    {
    case 'C':
        font->fontSpacing = CELL;
        break;
    case 'M':
        font->fontSpacing = MONOSPACED;
        break;
    case 'P':
        font->fontSpacing = PROPORTIONAL;
        break;
    }
}

static inline void handle_font_ascent(mstr ms, Font* font)
{
    assert(ms);
    assert(font);

    ms = nextWord(ms);
    u64 wordLen = wordLength(ms);
    mstr currentstr = alloca(wordLen+1);
    strncpy(currentstr, ms, wordLen);
    currentstr[wordLen] = '\0';
    font->fontAscent = atoi(currentstr);
}

static inline void handle_font_descent(mstr ms, Font* font)
{
    assert(ms);
    assert(font);

    ms = nextWord(ms);
    u64 wordLen = wordLength(ms);
    mstr currentstr = alloca(wordLen+1);
    strncpy(currentstr, ms, wordLen);
    currentstr[wordLen] = '\0';
    font->fontDescent = atoi(currentstr);
}

static inline void handle_endproperties(mstr ms, Font* font)
{
    // nothing has to happen
}

static inline void handle_chars(mstr ms, Font* font)
{
    assert(ms);
    assert(font);

    ms = nextWord(ms);
    u64 wordLen = wordLength(ms);
    mstr currentstr = alloca(wordLen+1);
    strncpy(currentstr, ms, wordLen);
    currentstr[wordLen] = '\0';
    font->amountOfCharacters = atoi(currentstr);
}

static inline u64 offsetTableSize(Font* font)
{
    assert(font->highestCharCode <= 255);
    return sizeof(u64) * (1 + font->highestCharCode);
}

static inline u64 characterDataSize(Font* font)
{
    return sizeof(Character) * font->amountOfCharacters;
}

inline u64 bitmapW(Font* font)
{
    assert(font);
    return (font->amountOfCharacters * (font->boundingBoxW + font->spacing) + font->spacing);
}

inline u64 bitmapH(Font* font)
{
    assert(font);
    return (font->boundingBoxH + 2 * font->spacing);
}


static inline u64 bitmapSize(Font* font)
{
    assert(font);
    return bitmapW(font) * bitmapH(font);
}

static u32 checkForHighestCharCode(FileData file)
{
    assert(file.ptr);

    mstr ms = file.ptr;

    i32 max = -1;

    do
    {
        if (wordsMatch("ENCODING", ms))
        {
            ms = nextWord(ms);
            u64 wordLen = wordLength(ms);
            mstr currentstr = alloca(wordLen+1);
            strncpy(currentstr, ms, wordLen);
            currentstr[wordLen] = '\0';
            i32 t = atoi(currentstr);
            if (t > max)
            {
                max = t;
            }
        }

        ms = newLine(ms);
    }
    while (!wordsMatch("ENDFONT", ms));

    assert(max != -1);
    return max;
}


Font InitializeFont(FileData file)
{
    assert(file.ptr);

    Font font;

    bool parametersRead[] = {
        false,
        false,
        false,
        false,
        false,
        false,
    };

    mstr ms = file.ptr;
    i32 paramNum = -1;
    do
    {
        paramNum = recogniseFontParameter((str)ms);
        if (paramNum != -1)
        {
            assert(parametersRead[paramNum] == false);
            parametersRead[paramNum] = true;
        }
        switch (paramNum)
        {
        case 0:
            handle_fontboundingbox(ms, &font);
            break;
        case 1:
            handle_spacing(ms, &font);
            break;
        case 2:
            handle_font_ascent(ms, &font);
            break;
        case 3:
            handle_font_descent(ms, &font);
            break;
        case 4:
            handle_endproperties(ms, &font);
            break;
        case 5:
            handle_chars(ms, &font);
            break;
        default:
            break;
        }

        ms = newLine(ms);
    }
    while (paramNum != 5);

    for range(i, 6)
    {
        assert(parametersRead[i]);
    }

    font.spacing = CHARACTER_SPACING;
    font.highestCharCode = checkForHighestCharCode(file);

    return font;
}

inline u64 GetSizeForCharacterData(Font* font)
{
    return
        offsetTableSize(font)
        + characterDataSize(font)
        + bitmapSize(font);
}

inline u64 GetSizeForEntireFont(Font* font)
{
    u64 size = sizeof(Font) + GetSizeForCharacterData(font);
    assert(size < MB);
    return size;
}

static i32 recogniseCharacterParameter(str s)
{
    assert(s);
    const u32 amountOfParameters = 6;
    str parameters[] = {
        "STARTCHAR",
        "ENCODING",
        "DWIDTH",
        "BBX",
        "BITMAP",
        "ENDCHAR",
    };

    for range(i, amountOfParameters)
    {
        if (wordsMatch(parameters[i], s))
        {
            return i;
        }
    }
    return -1;
}

static inline void handle_startchar(mstr ms, Character* character)
{
    assert(ms);
    assert(character);

    // nothing to do really
}

static inline void handle_encoding(mstr ms, Character* character)
{
    assert(ms);
    assert(character);

    ms = nextWord(ms);
    u64 wordLen = wordLength(ms);
    mstr currentstr = alloca(wordLen+1);
    strncpy(currentstr, ms, wordLen);
    currentstr[wordLen] = '\0';
    character->charCode = atoi(currentstr);
}

static inline void handle_dwidth(mstr ms, Character* character)
{
    assert(ms);
    assert(character);

    ms = nextWord(ms);
    u64 wordLen = wordLength(ms);
    mstr currentstr = alloca(wordLen+1);
    strncpy(currentstr, ms, wordLen);
    currentstr[wordLen] = '\0';
    character->dWidthX = atoi(currentstr);

    ms = nextWord(ms);
    wordLen = wordLength(ms);
    currentstr = alloca(wordLen+1);
    strncpy(currentstr, ms, wordLen);
    currentstr[wordLen] = '\0';
    character->dWidthY = atoi(currentstr);
}

static inline void handle_bbx(mstr ms, Character* character)
{
    assert(ms);
    assert(character);

    ms = nextWord(ms);
    u64 wordLen = wordLength(ms);
    mstr currentstr = alloca(wordLen+1);
    strncpy(currentstr, ms, wordLen);
    currentstr[wordLen] = '\0';
    character->boundingBoxW = atoi(currentstr);

    ms = nextWord(ms);
    wordLen = wordLength(ms);
    currentstr = alloca(wordLen+1);
    strncpy(currentstr, ms, wordLen);
    currentstr[wordLen] = '\0';
    character->boundingBoxH = atoi(currentstr);

    ms = nextWord(ms);
    wordLen = wordLength(ms);
    currentstr = alloca(wordLen+1);
    strncpy(currentstr, ms, wordLen);
    currentstr[wordLen] = '\0';
    character->offsetX = atoi(currentstr);

    ms = nextWord(ms);
    wordLen = wordLength(ms);
    currentstr = alloca(wordLen+1);
    strncpy(currentstr, ms, wordLen);
    currentstr[wordLen] = '\0';
    character->offsetY = atoi(currentstr);
}

//
u32 positionInBitmap(Font* font, u32 charIdx, u32 x, u32 y)
{
    assert(font);

    assert(x <= font->boundingBoxW);
    assert(y <= font->boundingBoxH);
    assert(charIdx <= font->amountOfCharacters);

    u32 yOffset = (font->amountOfCharacters * (font->spacing + font->boundingBoxW) + font->spacing) * (y + font->
        spacing);
    u32 xOffset = font->spacing + charIdx * (font->spacing + font->boundingBoxW) + x;

    return yOffset + xOffset;
}

static inline void handle_bitmap(mstr ms, Character* character, Font* font, u32 charIdx)
{
    assert(ms);
    assert(character);

    ms = nextWord(ms);
    u32 y = 0;
    while (!wordsMatch("ENDCHAR", ms))
    {
        // copying chars
        u32 wordLen = wordLength(ms);
        u8* word = alloca(wordLen);
        memcpy(word, ms, wordLen);

        // buffer for bytes representing bits
        u8* buf = alloca(wordLen * sizeof(u8));

        // extracting info from char's bits into u8's
        Error err;
        for range(i, wordLen)
        {
            u8 t = charToHex(word[i], &err);
            t = t << 4;
            for range(j, 4)
            {
                buf[4 * i + j] = (0x80 & t) == 0x80 ? 0xFF : 0x00;
                t = t << 1;
            }
        }

        for range(x, character->boundingBoxW)
        {
            u32 offset = positionInBitmap(font, charIdx, x, y);
            assert((u64)font->characterBitmap + (u64)offset <= (u64)font + GetSizeForEntireFont(font));
            // printf("accessing address %llx\n", (llu)&(font->characterBitmap[offset]));
            font->characterBitmap[offset] = buf[x];
        }

        y++;
        ms = newLine(ms);
    }
}

static inline void handle_endchar(mstr ms, Character* character)
{
    assert(ms);
    assert(character);

    // nothing more to do here
}


static Character ReadCharacterData(mstr ms, Font* font, u32 charIdx)
{
    assert(ms);

    Character character;

    bool parametersRead[] = {
        false,
        false,
        false,
        false,
        false,
        false,
    };

    i32 paramNum = -1;
    do
    {
        paramNum = recogniseCharacterParameter((str)ms);
        if (paramNum != -1)
        {
            assert(parametersRead[paramNum] == false);
            parametersRead[paramNum] = true;
        }
        switch (paramNum)
        {
        case 0:
            handle_startchar(ms, &character);
            break;
        case 1:
            handle_encoding(ms, &character);
            break;
        case 2:
            handle_dwidth(ms, &character);
            break;
        case 3:
            handle_bbx(ms, &character);
            break;
        case 4:
            handle_bitmap(ms, &character, font, charIdx);
            break;
        case 5:
            handle_endchar(ms, &character);
            break;
        default:
            break;
        }

        ms = newLine(ms);
    }
    while (paramNum != 5);

    for range(i, 6)
    {
        assert(parametersRead[i]);
    }

    return character;
}

Error InitializeCharacterDataOnAddress(Font* font, void* dest, FileData file)
{
    assert(font);
    assert(file.ptr);

    font->offsetTable = dest;
    font->charactersData = ((void*)font->offsetTable) + offsetTableSize(font);
    font->characterBitmap = ((void*)font->charactersData) + characterDataSize(font);

    assert((u64)font->offsetTable <= (u64)font + GetSizeForEntireFont(font));
    assert((u64)font->offsetTable > (u64)font);
    assert((u64)font->offsetTable + offsetTableSize(font) <= (u64)font + GetSizeForEntireFont(font));

    assert((u64)font->charactersData <= (u64)font + GetSizeForEntireFont(font));
    assert((u64)font->charactersData > (u64)font);
    assert((u64)font->charactersData + characterDataSize(font) <= (u64)font + GetSizeForEntireFont(font));

    assert((u64)font->characterBitmap <= (u64)font + GetSizeForEntireFont(font));
    assert((u64)font->characterBitmap > (u64)font);
    assert((u64)font->characterBitmap + bitmapSize(font) <= (u64)font + GetSizeForEntireFont(font));

    memset(font->offsetTable, 0, GetSizeForCharacterData(font));

    printf("Font addresses: %llx - %llx\n", (llu)font, (llu)font + GetSizeForEntireFont(font));

    mstr ms = (mstr)file.ptr;

    memset(font->offsetTable, -1, offsetTableSize(font));
    for range(i, font->amountOfCharacters)
    {
        do
        {
            ms = newLine(ms);
        }
        while (!wordsMatch("STARTCHAR", ms));

        font->charactersData[i] = ReadCharacterData(ms, font, i);
        font->offsetTable[font->charactersData[i].charCode] = i;
    }

    return OK;
}

// The same as InitializeCharacterDataOntoFont but will destination is top of the address just on top  of font
Error InitializeCharacterDataOntoFont(Font* font, FileData file)
{
    return InitializeCharacterDataOnAddress(
    	font,
     	((void*)font) + sizeof(Font),
      	file
    );
}

v2 GetTextureCoordinateBottomLeft(
    u8 character,
    Font* font
)
{
    assert(font);
    assert(font->offsetTable[character] != -1);

    u64 bitmapWidth = bitmapW(font);
    u64 bitmapHeight = bitmapH(font);

    u32 pixelOffsetX =
        CHARACTER_SPACING * (font->offsetTable[character] + 1) +
        font->offsetTable[character] * font->charactersData[font->offsetTable[character]].boundingBoxW;

    u32 pixelOffsetY = CHARACTER_SPACING +
        font->charactersData[font->offsetTable[character]].boundingBoxH;

    v2 position = {
        .x = (f64)pixelOffsetX / bitmapWidth,
        .y = (f64)pixelOffsetY / bitmapHeight,
    };

    // printf("bottom left pixels:\t x: %d y: %d\nfloats:\t x: %f y: %f\n", pixelOffsetX, pixelOffsetY, position.x, position.y);

    return position;
}

v2 GetTextureCoordinateBottomRight(
    u8 character,
    Font* font
)
{
    assert(font);
    assert(font->offsetTable[character] != -1);

    u64 bitmapWidth = bitmapW(font);
    u64 bitmapHeight = bitmapH(font);

    u32 pixelOffsetX =
        CHARACTER_SPACING * (font->offsetTable[character] + 1) +
        (font->offsetTable[character] + 1) * font->charactersData[font->offsetTable[character]].boundingBoxW;

    u32 pixelOffsetY = CHARACTER_SPACING +
        font->charactersData[font->offsetTable[character]].boundingBoxH;

    v2 position = {
        .x = (f64)pixelOffsetX / bitmapWidth,
        .y = (f64)pixelOffsetY / bitmapHeight,
    };

    // printf("bottom right pixels:\t x: %d y: %d\nfloats:\t x: %f y: %f\n", pixelOffsetX, pixelOffsetY, position.x, position.y);

    return position;
}

v2 GetTextureCoordinateTopLeft(
    u8 character,
    Font* font
)
{
    assert(font);
    assert(font->offsetTable[character] != -1);

    u64 bitmapWidth = bitmapW(font);
    u64 bitmapHeight = bitmapH(font);

    u32 pixelOffsetX =
        CHARACTER_SPACING * (font->offsetTable[character] + 1) +
        font->offsetTable[character] * font->charactersData[font->offsetTable[character]].boundingBoxW;

    u32 pixelOffsetY = CHARACTER_SPACING;

    v2 position = {
        .x = (f64)pixelOffsetX / bitmapWidth,
        .y = (f64)pixelOffsetY / bitmapHeight,
    };

    // printf("top left pixels:\t x: %d y: %d\nfloats:\t x: %f y: %f\n", pixelOffsetX, pixelOffsetY, position.x, position.y);

    return position;
}


v2 GetTextureCoordinateTopRight(
    u8 character,
    Font* font
)
{
    assert(font);
    assert(font->offsetTable[character] != -1);

    u64 bitmapWidth = bitmapW(font);
    u64 bitmapHeight = bitmapH(font);

    u32 pixelOffsetX =
        CHARACTER_SPACING * (font->offsetTable[character] + 1) +
        (font->offsetTable[character] + 1) * font->charactersData[font->offsetTable[character]].boundingBoxW;

    u32 pixelOffsetY = CHARACTER_SPACING;

    v2 position = {
        .x = (f64)pixelOffsetX / bitmapWidth,
        .y = (f64)pixelOffsetY / bitmapHeight,
    };

    // printf("top right pixels:\t x: %d y: %d\nfloats:\t x: %f y: %f\n", pixelOffsetX, pixelOffsetY, position.x, position.y);

    return position;
}


v2 GetCharacterDOffset(
    u8 character,
    Font* font
)
{
    assert(font);

    v2 doffset = {
        .x = font->charactersData[font->offsetTable[character]].dWidthX,
        .y = font->charactersData[font->offsetTable[character]].dWidthY,
    };
    return doffset;
}

const u32 vertexStride = 5;
const u32 characterStride = vertexStride * 4;
const u32 uvOffset = 2;
const u32 LineIdxOffset = 4;

void FillInFontTexture(
    str sourceString,
    u32 stringLength,
    Font* font,
    f32* out // array bo filled in
)
{
    assert(sourceString);
    assert(font);
    assert(out);

    // bottom left
    for range(i, stringLength)
    {
        v2 uv = GetTextureCoordinateBottomLeft(sourceString[i], font);
        out[characterStride * i + 0 * vertexStride + uvOffset] = uv.x;
        out[characterStride * i + 0 * vertexStride + uvOffset + 1] = uv.y;
    }
    // bottom right
    for range(i, stringLength)
    {
        v2 uv = GetTextureCoordinateBottomRight(sourceString[i], font);
        out[characterStride * i + 1 * vertexStride + uvOffset] = uv.x;
        out[characterStride * i + 1 * vertexStride + uvOffset + 1] = uv.y;
    }
    // top right
    for range(i, stringLength)
    {
        v2 uv = GetTextureCoordinateTopRight(sourceString[i], font);
        out[characterStride * i + 2 * vertexStride + uvOffset] = uv.x;
        out[characterStride * i + 2 * vertexStride + uvOffset + 1] = uv.y;
    }
    // top left
    for range(i, stringLength)
    {
        v2 uv = GetTextureCoordinateTopLeft(sourceString[i], font);
        out[characterStride * i + 3 * vertexStride + uvOffset] = uv.x;
        out[characterStride * i + 3 * vertexStride + uvOffset + 1] = uv.y;
    }
}

void FillInScreenspacePosition(
    str sourceString,
    u32 stringLength,
    u32 fontScale, // how much do i scale the font
    Font* font,
    f32* out // array bo filled in
)
{
    assert(sourceString);
    assert(font);
    assert(out);

    // creating table of pixel offsets for characters to make them string on screen
    v2u32* needleoffsets = alloca(stringLength * sizeof(v2u32));
    v2u32 needle = {
        .x = 0,
        .y = 0
    };
    for range(i, stringLength)
    {
        needleoffsets[i] = needle;
        needle.x += font->charactersData[font->offsetTable[(i8)sourceString[i]]].dWidthX;
        needle.y += font->charactersData[font->offsetTable[(i8)sourceString[i]]].dWidthY;
    }

    // bottom left
    for range(i, stringLength)
    {
        v2u32 pos = {
            .x = 0,
            .y = font->boundingBoxH
        };
        pos.x += needleoffsets[i].x;
        pos.y += needleoffsets[i].y;

        pos.x *= fontScale;
        pos.y *= fontScale;


        v2 screenspacePos = PixelsToScreenspace(pos);

        out[characterStride * i + 0 * vertexStride] = screenspacePos.x;
        out[characterStride * i + 0 * vertexStride + 1] = screenspacePos.y;
    }

    // bottom right
    for range(i, stringLength)
    {
        v2u32 pos = {
            .x = font->boundingBoxW,
            .y = font->boundingBoxH
        };
        pos.x += needleoffsets[i].x;
        pos.y += needleoffsets[i].y;

        pos.x *= fontScale;
        pos.y *= fontScale;

        v2 screenspacePos = PixelsToScreenspace(pos);

        out[characterStride * i + 1 * vertexStride] = screenspacePos.x;
        out[characterStride * i + 1 * vertexStride + 1] = screenspacePos.y;
    }

    // top right
    for range(i, stringLength)
    {
        v2u32 pos = {
            .x = font->boundingBoxW,
            .y = 0
        };
        pos.x += needleoffsets[i].x;
        pos.y += needleoffsets[i].y;

        pos.x *= fontScale;
        pos.y *= fontScale;

        v2 screenspacePos = PixelsToScreenspace(pos);

        out[characterStride * i + 2 * vertexStride] = screenspacePos.x;
        out[characterStride * i + 2 * vertexStride + 1] = screenspacePos.y;
    }

    // top left
    for range(i, stringLength)
    {
        v2u32 pos = {
            .x = 0,
            .y = 0
        };
        pos.x += needleoffsets[i].x;
        pos.y += needleoffsets[i].y;

        pos.x *= fontScale;
        pos.y *= fontScale;

        v2 screenspacePos = PixelsToScreenspace(pos);

        out[characterStride * i + 3 * vertexStride] = screenspacePos.x;
        out[characterStride * i + 3 * vertexStride + 1] = screenspacePos.y;
    }
}

// fills in indexes for ebo
void FillInVertexIndicies(
    u32 stringLength,
    u32 amountOfExistingLetters,
    u32* out // array bo filled in
)
{
    assert(out);

    const u32 indexToAdd = amountOfExistingLetters * 4;

    for range(i, stringLength)
    {
        out[6 * i + 0] = i * 4 + 0 + indexToAdd;
        out[6 * i + 1] = i * 4 + 1 + indexToAdd;
        out[6 * i + 2] = i * 4 + 3 + indexToAdd;
        out[6 * i + 3] = i * 4 + 1 + indexToAdd;
        out[6 * i + 4] = i * 4 + 2 + indexToAdd;
        out[6 * i + 5] = i * 4 + 3 + indexToAdd;
    }
}

void FillInLineIndicies(
    u32 stringLength,
    u32 lineIdx,
    u32* out // array to be filled in
)
{
    assert(out);

    u32* needle = out + LineIdxOffset;
    for range(i, stringLength*4)
    {
        *needle = lineIdx;
        needle += vertexStride;
    }
}

void InitializeTextureCoordinatesBuffer(
    str sourceString, // ascii line to generate
    u32 stringLength,
    u32 fontScale,
    u32 lineIdx,
    Font* font, // font to look up to
    f32* VBO_out,
    u32* EBO_out
)
{
    assert(font);
    assert(sourceString);
    assert(VBO_out);

    FillInScreenspacePosition(
        sourceString,
        stringLength,
        fontScale,
        font,
        VBO_out
    );

    FillInFontTexture(
        sourceString,
        stringLength,
        font,
        VBO_out
    );

    FillInLineIndicies(
        stringLength,
        lineIdx,
        (u32*)VBO_out
    );

    FillInVertexIndicies(
        stringLength,
        0,
        EBO_out
    );
}

// how many bytes will VBO occupy
u32 SpaceNeededForVBO(
    u32 stringLenght
)
{
    return characterStride * sizeof(f32) * stringLenght;
}

// how many bytes will EBO occupy
u32 SpaceNeededForEBO(
    u32 stringLenght
)
{
    return 6 * sizeof(u32) * stringLenght;
}
