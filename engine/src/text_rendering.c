#include <assert.h>
#include <engine/text_rendering.h>

void InitializeTextRenderingObject(
	void* base
) {
	assert(base);

	TextData* textData = base;
	*textData = (TextData){
		.totalCapacity = TOTAL_SIZE_FOR_TEXT_RENDERING - sizeof(TextData),
		.amountOfLines = 0,

		.textCapacity = MAX_SIZE_FOR_TEXT,
		.textStart = base + sizeof(TextData),
		.textTop = base + sizeof(TextData),

		.verticiesCapacity = MAX_AMOUNT_OF_CHARS_PER_SCENE * SIZEOF_ONE_LETTER_VERTICIES,
		.verticiesStart = base + sizeof(TextData) + MAX_SIZE_FOR_TEXT,
		.verticiesTop = base + sizeof(TextData) + MAX_SIZE_FOR_TEXT,

		.indiciesCapacity = MAX_AMOUNT_OF_CHARS_PER_SCENE * SIZEOF_ONE_LETTER_INDEXES,
		.indiciesStart = base + sizeof(TextData) + MAX_SIZE_FOR_TEXT + MAX_AMOUNT_OF_CHARS_PER_SCENE * SIZEOF_ONE_LETTER_VERTICIES,
		.indiciesTop = base + sizeof(TextData) + MAX_SIZE_FOR_TEXT + MAX_AMOUNT_OF_CHARS_PER_SCENE * SIZEOF_ONE_LETTER_VERTICIES,
	};


}