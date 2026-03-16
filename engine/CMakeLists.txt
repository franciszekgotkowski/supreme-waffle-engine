add_library(engine STATIC)

set(ENGINE_SOURCES
		src/platform/window/window_glfw.c
		src/platform/input/input_glfw.c
		src/platform/game_loop/game_loop_glfw.c
		src/platform/graphics/graphics_glad.c
		src/platform/shader/shader_glad.c
		src/memory_pool.c
		src/string_utils.c
		src/memory_arena.c
		src/font.c
		src/scene.c
		src/ui.c
		src/area.c
		src/memory_region_capacities.c
		src/input.c
		src/measure_time.c
		src/text_rendering.c
)

if(UNIX)
	target_compile_definitions(engine PRIVATE PLATFORM_LINUX)
	list(APPEND
			ENGINE_SOURCES
			src/platform/memory/memory_linux.c
			src/platform/measure_time/measure_time_linux.c
			src/platform/file_io/file_io_linux.c
	)
	target_link_directories(engine PUBLIC
			${CMAKE_CURRENT_SOURCE_DIR}/libs/linux/dynamic
			${CMAKE_CURRENT_SOURCE_DIR}/libs/linux/static
	)
elseif(WIN32)
	target_compile_definitions(engine PRIVATE PLATFORM_WINDOWS)
	list(APPEND
			ENGINE_SOURCES
			src/platform/memory/memory_windows.c
			# src/platform/measure_time/measure_time_windows.c
			src/platform/file_io/file_io_windows.c
	)
	target_link_directories(engine PUBLIC
			${CMAKE_CURRENT_SOURCE_DIR}/libs/windows/dynamic
			${CMAKE_CURRENT_SOURCE_DIR}/libs/windows/static
	)
endif()

target_sources(engine PRIVATE ${ENGINE_SOURCES})

target_include_directories(engine PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/headers
)
target_link_libraries(engine
		PUBLIC
		glad
		stb
		cglm
		glfw3
)

if(UNIX)
	target_link_libraries(engine PRIVATE
			m
			dl
	)
elseif(WIN32)
	target_link_libraries(engine PRIVATE
			opengl32
	)
endif()



add_library(glad STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/libs/src/glad/glad.c
)

target_include_directories(glad PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/headers/external
)

add_library(stb STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/libs/src/stb/stb_image.c
	${CMAKE_CURRENT_SOURCE_DIR}/libs/src/stb/stb_image_write.c
)

target_include_directories(stb PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/headers/external
)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/libs/src/cglm
				# EXCLUDE_FROM_ALL
)

# add_library(cjson STATIC
# 	${CMAKE_CURRENT_SOURCE_DIR}/libs/src/cJSON/cJSON.c
# )
#
# target_include_directories(cjson SYSTEM PUBLIC
# 	${CMAKE_CURRENT_SOURCE_DIR}/headers/external
# )
#
# add_library(nuklear STATIC
# 	${CMAKE_CURRENT_SOURCE_DIR}/libs/src/nuklear/nuklear.c
# )
#
# target_include_directories(nuklear SYSTEM PUBLIC
# 	${CMAKE_CURRENT_SOURCE_DIR}/headers/external
# )
