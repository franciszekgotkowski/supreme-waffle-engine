add_library(engine STATIC)

set(ENGINE_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/src/memory_pool.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/memory_arena.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/font.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/scene.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/ui.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/area.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/memory_region_capacities.c
        ${CMAKE_CURRENT_SOURCE_DIR}/src/handle_input.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/text_rendering.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/window.c
)

target_sources(engine PRIVATE ${ENGINE_SOURCES})

target_include_directories(engine PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/headers

		${CMAKE_SOURCE_DIR}/platform/headers/export
		${CMAKE_SOURCE_DIR}/common/headers

)

target_link_libraries(engine
		PRIVATE
		platform
		cglm
		common
)

# add_library(stb STATIC
# 	${CMAKE_CURRENT_SOURCE_DIR}/libs/src/stb/stb_image.c
# 	${CMAKE_CURRENT_SOURCE_DIR}/libs/src/stb/stb_image_write.c
# )

# target_include_directories(stb PRIVATE
# 	${CMAKE_CURRENT_SOURCE_DIR}/headers/external
# )

# add_subdirectory(
# 	${CMAKE_CURRENT_SOURCE_DIR}/libs/src/cglm
# )
