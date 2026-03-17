#pragma once

#define FONT_FILES_LOCATION "../../assets/fonts/bdf/"
#define CHARACTER_SPACING 1

#include <engine/typedefs.h>
#include <engine/errors.h>
#include <engine/platform/file_io.h>

typedef enum {
	CELL, 			// every character has the same width
	MONOSPACED,  	// every character has the same width but characters are treated like elements of a grid
	PROPORTIONAL	// every character has different size
} FontSpacing ;

typedef struct {
	// ascii code
	// from ENCODING
	u32 charCode;

	// how much cursor should move after drawing this character
	// from DWIDTH
	u32 dWidthX;
	u32 dWidthY;

	// bounding box size
	// from BBX
	u32 boundingBoxW;
	u32 boundingBoxH;

	// offsets from baseline
	// from BBX
	i32 offsetX;
	i32 offsetY;
} Character ;

// Contains all relevant info about a font
typedef struct {
	// maximum bounding box size
	// from FONTBOUNDINGBOX
	u32 boundingBoxW;
	u32 boundingBoxH;

	// maximum offsets from baseline
	// from FONTBOUNDINGBOX
	i32 offsetX;
	i32 offsetY;

	// type of font spacing
	// from SPACING
	FontSpacing fontSpacing;

	// font ascent - how many pixels are below and above baseline
	// from FONT_ASCENT
	u32 fontAscent;
	// from FONT_DESCENT
	u32 fontDescent;

	// amount of characters
	// from CHARS
	u32 amountOfCharacters;

	// Highest char code. Needed for me to create a table of offsets
	u32 highestCharCode;

    // How many pixels will be between character bitmaps
    // All characters are in 1 array hegth array (there are no columns of characters. It has amountOfCharacters x 1 proportions)
    u32 spacing;

    // index [i] is offset in to array which has offset into charactersData which has relevant information about the character
    i32* offsetTable;

    // array with relevant information about character
    Character* charactersData;

    // bitmap of the font
	u8* characterBitmap;
} Font;

// size in bytes needed to accomodate offsetTable, charactersData and characterBitmap
u64 GetSizeForCharacterData(Font* font);

// size in bytes for GetSizeForCharacterData and Font struct
u64 GetSizeForEntireFont(Font* font);

// Creates Font struct with info about a font without filling in offsetTable, charactersData and characterBitmap
// This will happen later in InitializeCharacterDataOntoFont or InitializeCharacterDataOnAddress
Font InitializeFont(
	FileData file
);

// Function gives font struct pointers to where character structs are and where a bitmap is.
// Inteded location of those is next to the Font struct like here:
// 		[low address]
// 		Font struct
// 		Offset table where indexes of the table are charCodes
// 		Character struct[]
// 		Character bitmaps[]
// 		[hight address]
// NOTE: if character does not exist there is -1 in its offset table value
Error InitializeCharacterDataOnAddress(
	Font* font,
	void* dest,
	FileData file
);

// the same as InitializeCharacterDataOntoFont but it allocates character data not on some address but on addres adter font
Error InitializeCharacterDataOntoFont(
	Font* font,
	FileData file
);

// how wide is bitmap in pixels accounting for padding
u64 bitmapW(
	Font* font
);

// how high is bitmap in pixels accounting for padding
u64 bitmapH(
	Font* font
);

// gets bottom left u8acter position in texture
v2 GetTextureCoordinateBottomLeft(
	u8 character,
	Font* font
);

// gets bottom right u8acter position in texture
v2 GetTextureCoordinateBottomRight(
	u8 character,
	Font* font
);

// gets top left u8acter position in texture
v2 GetTextureCoordinateTopLeft(
	u8 character,
	Font* font
);

// gets top left u8acter position in texture
v2 GetTextureCoordinateTopRight(
	u8 character,
	Font* font
);

v2 GetCharacterDOffset(
	u8 character,
	Font* font
);

// for each character there will be one 4 edges each havind 4 floats and then after each egde will have their u32 offset for ebo
// Each one will have counter clockwise points
// and each point will be comprised of:
// position x, y in screenspace (floats) and x, y for texture mapping
// edges order: bottom left, bottom right, top right, top left
void InitializeTextureCoordinatesBuffer(
	str sourceString, // ascii line to generate
	u32 stringLength,
	u32 fontScale, // by how much should font be scaled
	u32 lineIdx,
	Font* font, // font to look up to
	f32* VBO_out,
	u32* EBO_out
);

//DATA LAYOUT IN VERTEX BUFFER:
// first character:
// 		bottom left egde:
// 			f32 screenspace pos x
// 			f32 screenspace pos y
// 			f32 uv position x
// 			f32 uv pos y
// 			u32 index of line
// 		bottom right egde:
//			...
// 		top right egde:
// 			...
// 		top left egde:
// 			...
// second character:
// ....

// fill in data under array with screenspace positions of characters
void FillInScreenspacePosition(
	str sourceString,
	u32 stringLength,
	u32 fontScale,
	Font* font,
	f32* out // array to be filled in
);

// fill in data under array with texture of font
void FillInFontTexture(
	str sourceString,
	u32 stringLength,
	Font* font,
	f32* out // array to be filled in
);

// fills in line index for vbo
void FillInLineIndicies(
	u32 stringLength,
	u32 lineIdx,
	u32* out // array to be filled in
);

// fills in indexes for ebo
void FillInVertexIndicies(
	str sourceString,
	u32 stringLength,
	u32 amountOfExistingLetters,
	u32* out // array to be filled in
);

// how many bytes will VBO occupy
u32 SpaceNeededForVBO(
	u32 stringLenght
);

// how many bytes will EBO occupy
u32 SpaceNeededForEBO(
	u32 stringLenght
);