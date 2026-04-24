#include <engine/font.h>
#include <common/typedefs.h>
#include <engine/text_rendering.h>
#include <assert.h>
#include <common/errors.h>
#include <string.h>

void InitializeTextRenderingObject(
	void* ptr
) {

	assert(ptr);

	TextData* textData = (TextData*)ptr;

	*textData = (TextData){
		.totalCapacity = TOTAL_SIZE_FOR_TEXT_RENDERING - sizeof(TextData),
		.amountOfLines = 0,
		.amountOfCharacters = 0,

		.textCapacity = MAX_SIZE_FOR_TEXT,
		.textStart = ptr + sizeof(TextData),
		.textTop = ptr + sizeof(TextData),

		.verticiesCapacity = MAX_AMOUNT_OF_CHARS_PER_SCENE * SIZEOF_ONE_LETTER_VERTICIES,
		.verticiesStart = ptr + sizeof(TextData) + MAX_SIZE_FOR_TEXT,
		.verticiesTop = ptr + sizeof(TextData) + MAX_SIZE_FOR_TEXT,

		.indiciesCapacity = MAX_AMOUNT_OF_CHARS_PER_SCENE * SIZEOF_ONE_LETTER_INDEXES,
		.indiciesStart = ptr + sizeof(TextData) + MAX_SIZE_FOR_TEXT + MAX_AMOUNT_OF_CHARS_PER_SCENE * SIZEOF_ONE_LETTER_VERTICIES,
		.indiciesTop = ptr + sizeof(TextData) + MAX_SIZE_FOR_TEXT + MAX_AMOUNT_OF_CHARS_PER_SCENE * SIZEOF_ONE_LETTER_VERTICIES,
	};
}

Error AppendNewLine(
	str sourceString,
	u32 letterCount,
	TextData* textData,

	Font* font,
	v2 offset,
	Color color,
	u32 scale
) {
	assert(sourceString);
	assert(textData);

	assert(textData->amountOfCharacters <= MAX_AMOUNT_OF_CHARS_PER_SCENE);
	assert(textData->amountOfLines <= MAX_AMOUNT_OF_LINES_IN_SCENE);

	assert(font);

	if (textData->amountOfLines + 1 > MAX_AMOUNT_OF_LINES_IN_SCENE) {
		return OUT_OF_INDEXES;
	}
	if (textData->amountOfCharacters + letterCount > MAX_AMOUNT_OF_CHARS_PER_SCENE) {
		return OUT_OF_MEMORY;
	}

	textData->linesData.offset[textData->amountOfLines] = offset;
	textData->linesData.color[textData->amountOfLines] = color;
	textData->linesData.scale[textData->amountOfLines] = scale;

	textData->linesData.textPtr[textData->amountOfLines] = textData->textTop;
	textData->linesData.verticiesPtr[textData->amountOfLines] = textData->verticiesTop;
	textData->linesData.indiciesPtr[textData->amountOfLines] = textData->indiciesTop;

	textData->linesData.letterCount[textData->amountOfLines] = letterCount;
	textData->linesData.shouldDraw[textData->amountOfLines] = true;

	// textData->line[textData->amountOfLines] = (Line){
	// 	.offset = offset,
	// 	.color = color,
	// 	.scale = scale,

	// 	.textPtr = textData->textTop,
	// 	.verticiesPtr = textData->verticiesTop,
	// 	.indiciesPtr = textData->indiciesTop,

	// 	.letterCount = letterCount,
	// 	.shouldDraw = true
	// };

	FillInScreenspacePosition(
		sourceString,
		letterCount,
		scale,
		font,
		// textData->line[textData->amountOfLines].verticiesPtr
		textData->linesData.verticiesPtr[textData->amountOfLines]
	);

	FillInFontTexture(
		sourceString,
		letterCount,
		font,
		// textData->line[textData->amountOfLines].verticiesPtr
		textData->linesData.verticiesPtr[textData->amountOfLines]
	);

	FillInLineIndicies(
		letterCount,
		textData->amountOfLines,
		// (u32*)textData->line[textData->amountOfLines].verticiesPtr
		(u32*)textData->linesData.verticiesPtr[textData->amountOfLines]
	);

	FillInVertexIndicies(
		letterCount,
		textData->amountOfCharacters,
		// textData->line[textData->amountOfLines].indiciesPtr
		textData->linesData.indiciesPtr[textData->amountOfLines]
	);

	memcpy(textData->textTop, sourceString, letterCount);

	textData->amountOfLines += 1;
	textData->amountOfCharacters += letterCount;
	textData->textTop += letterCount;
	textData->verticiesTop += SpaceNeededForVBO(letterCount);
	textData->indiciesTop += SpaceNeededForEBO(letterCount);

	// assert(NOT_COMPLETED);
	return OK;
}