add_library(platform STATIC)

set(PLATFORM_SOURCES
		src/window/window_glfw.c
		src/input/input_glfw.c
		src/game_loop/game_loop_glfw.c
		src/graphics/graphics_glad.c
		src/shader/shader_glad.c
)

if(UNIX)
	target_compile_definitions(platform PRIVATE PLATFORM_LINUX)
	list(APPEND
			PLATFORM_SOURCES
			src/memory/memory_linux.c
			src/measure_time/measure_time_linux.c
			src/file_io/file_io_linux.c
	)
	target_link_directories(platform PRIVATE
			# ${CMAKE_CURRENT_SOURCE_DIR}/libs/linux/dynamic
			${CMAKE_CURRENT_SOURCE_DIR}/libs/linux/static
	)
elseif(WIN32)
	target_compile_definitions(platform PRIVATE PLATFORM_WINDOWS)
	list(APPEND
			PLATFORM_SOURCES
			src/memory/memory_windows.c
			# src/measure_time/measure_time_windows.c
			src/file_io/file_io_windows.c
	)
	target_link_directories(platform PRIVATE
			# ${CMAKE_CURRENT_SOURCE_DIR}/libs/windows/dynamic
			${CMAKE_CURRENT_SOURCE_DIR}/libs/windows/static
	)
endif()

target_sources(platform PRIVATE ${PLATFORM_SOURCES})

target_include_directories(platform PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/headers
		${CMAKE_SOURCE_DIR}/primitives/headers
)
target_link_libraries(platform
		PUBLIC
		glad
		glfw3
		primitives
)

if(UNIX)
	target_link_libraries(platform PRIVATE
			m
			dl
	)
elseif(WIN32)
	target_link_libraries(platform PRIVATE
			opengl32
	)
endif()



add_library(glad STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/libs/src/glad/glad.c
)

target_include_directories(glad PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/headers/external
)

# add_library(stb STATIC
# 	${CMAKE_CURRENT_SOURCE_DIR}/libs/src/stb/stb_image.c
# 	${CMAKE_CURRENT_SOURCE_DIR}/libs/src/stb/stb_image_write.c
# )

# target_include_directories(stb PUBLIC
# 	${CMAKE_CURRENT_SOURCE_DIR}/headers/external
# )

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/libs/src/cglm
				# EXCLUDE_FROM_ALL
)
