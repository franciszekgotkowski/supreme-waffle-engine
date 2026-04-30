add_library(common STATIC)

target_compile_options(common PRIVATE
	${COMMON_FLAGS}
	$<$<CONFIG:Debug>:${DEBUG_FLAGS}>
	$<$<CONFIG:Release>:${RELEASE_FLAGS}>
)

set(COMMON_SOURCES
	src/string_utils.c
	src/memory_arena.c
)

target_sources(common PRIVATE
	${COMMON_SOURCES}
)

target_include_directories(common PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/headers
)
