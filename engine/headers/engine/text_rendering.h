#include <engine/typedefs.h>
#include <engine/platform/memory_allocations.h>
#include <engine/errors.h>
#include <engine/scene.h>
#include <engine/font.h>
#include <stdbool.h>

#define MAX_AMOUNT_OF_CHARS_PER_SCENE (10*KB)
#define MAX_AMOUNT_OF_LINES_IN_SCENE (1*KB)

#define MAX_AMOUNT_OF_FONTS_PER_SCENE 10
#define MAX_SIZE_FOR_SINGLE_FONT (KB*50)
#define SIZEOF_MEMORY_FOR_FONTS (MAX_AMOUNT_OF_FONTS_PER_SCENE*MAX_SIZE_FOR_SINGLE_FONT)

// TEXT DEFINES
#define MAX_SIZE_FOR_TEXT (MAX_AMOUNT_OF_CHARS_PER_SCENE*sizeof(char))
#define MAX_SIZE_FOR_TEXT_RENDER_DATA (sizeof(TextData))

// sizeof float coordinate *
// amount of floats in creenspace position  *
// amount of floats in texture positionspace *
// amount of verticies +
// sizeof line index
#define SIZEOF_ONE_LETTER_VERTICIES (\
	sizeof(f32)*\
	2*\
	2*\
	4+\
	sizeof(u32))

// sizeof idexes *
// amount of indexes per letter
#define SIZEOF_ONE_LETTER_INDEXES (\
	sizeof(u32)*\
	6\
)

// ilość charów *
// floaty dla xy dla koordynatów na ekranie + koordynatów tekstury
// + indeksy EBO
// indeksy linijek
#define MAX_SIZE_FOR_CHARACTER_RENDER_DATA (\
	MAX_AMOUNT_OF_CHARS_PER_SCENE*\
	(SIZEOF_ONE_LETTER_VERTICIES + SIZEOF_ONE_LETTER_INDEXES)\
)

// How much space is needed for text rendering (for text but also memory needed for rendering)
#define TOTAL_SIZE_FOR_TEXT_RENDERING (\
	MAX_SIZE_FOR_TEXT+\
	MAX_SIZE_FOR_TEXT_RENDER_DATA+\
	MAX_SIZE_FOR_CHARACTER_RENDER_DATA\
)


typedef union {
	struct {
		f32 r;
		f32 g;
		f32 b;
		f32 a;
	};
	f32 rgba[4];
	f32 rgb[3];
} Color ;

// // contains data for each line
// typedef struct {
// 	v2 offset;
// 	Color color;
// 	u32 scale;

// 	mstr* textPtr;
// 	void* verticiesPtr;
// 	void* indiciesPtr;
// 	u32 letterCount;

// 	bool shouldDraw;
// } Line ;

typedef struct {
	v2 offset[MAX_AMOUNT_OF_LINES_IN_SCENE];
	Color color[MAX_AMOUNT_OF_LINES_IN_SCENE];
	u32 scale[MAX_AMOUNT_OF_LINES_IN_SCENE];
	u32 shouldDraw[MAX_AMOUNT_OF_LINES_IN_SCENE];

	mstr* textPtr[MAX_AMOUNT_OF_LINES_IN_SCENE];
	void* verticiesPtr[MAX_AMOUNT_OF_LINES_IN_SCENE];
	void* indiciesPtr[MAX_AMOUNT_OF_LINES_IN_SCENE];
	u32 letterCount[MAX_AMOUNT_OF_LINES_IN_SCENE];
} LinesData ;

typedef struct {
	u64 totalCapacity;

	// Line line[MAX_AMOUNT_OF_LINES_IN_SCENE];
	LinesData linesData;
	u32 amountOfLines;
	u32 amountOfCharacters;

	u64 textCapacity;
	void* textStart;
	void* textTop;

	u64 verticiesCapacity;
	void* verticiesStart;
	void* verticiesTop;

	u64 indiciesCapacity;
	void* indiciesStart;
	void* indiciesTop;
} TextData;

// Initializes memory regions (asset and gameObject) for text rendering
Error InitializeTextRenderingObject(
	u32 gameObjectIdx,
	SceneData* sceneData
);

// function appends new line into text game object
// Line lenght should not include \0 character because it will not be displayed anyway
// Function can return errors:
// 		OUT_OF_MEMORY - if scene cannot accomodate those characters
// 		OUT_OF_INDEXES - if scene cannot accomodate this line
// 		OK
Error AppendNewLine(
	str line,
	u32 letterCount,
	TextData* textData,

	Font* font,
	v2 offset,
	Color color,
	u32 scale
);

void DeleteLine(
	u32 lineIdx
);