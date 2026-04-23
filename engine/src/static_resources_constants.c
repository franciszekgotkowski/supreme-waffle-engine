#include <engine/static_resources.h>
#include <common/typedefs.h>

// Default amounts of elements each indexer can handle.
// Unfortunately i can set values for all static resources.
// This may seem as a problem but really is not for 2 reasons:
// 		- default value is 0 and i can assert for that and know if an index is wrong
//		- it is only for InitializeStaticResourceIndexer function
const u32 DefaultAmountsOfIndexes[AMOUNT_OF_STATIC_SCENE_RESOURCES] = {
	[FONT_INDEXER] = 100,
	[OBJECT_3D_INDEXER] = 10000,
};

const u32 SizesForEachStaticResource[AMOUNT_OF_STATIC_SCENE_RESOURCES] = {
	[TEXT_RENDERING_OBJECT] = TOTAL_SIZE_FOR_TEXT_RENDERING,
	[FONT_INDEXER] = GetWholeIndexerSize(DefaultAmountsOfIndexes[FONT_INDEXER]),
	[OBJECT_3D_INDEXER] = GetWholeIndexerSize(DefaultAmountsOfIndexes[OBJECT_3D_INDEXER]),
};
