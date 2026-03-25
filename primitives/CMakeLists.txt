add_library(primitives STATIC)

set(PRIMITIVES_SOURCES
	src/string_utils.c
)

target_sources(primitives PRIVATE
	${PRIMITIVES_SOURCES}
)

target_include_directories(primitives PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/headers
)
