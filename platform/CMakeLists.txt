add_library(platform STATIC)

target_compile_options(platform PRIVATE
	${COMMON_FLAGS}
	$<$<CONFIG:Debug>:${DEBUG_FLAGS}>
	$<$<CONFIG:Release>:${RELEASE_FLAGS}>
)

set(PLATFORM_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/src/window/window_glfw.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/collect_input/collect_input_glfw.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/game_loop/game_loop_glfw.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/graphics_glad.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/shader/shader_glad.c
		${CMAKE_CURRENT_SOURCE_DIR}/src/measure_time/measure_time.c
)

if(UNIX)
	target_compile_definitions(platform PRIVATE PLATFORM_LINUX)
	list(APPEND PLATFORM_SOURCES
			${CMAKE_CURRENT_SOURCE_DIR}/src/memory/memory_linux.c
			${CMAKE_CURRENT_SOURCE_DIR}/src/measure_time/measure_time_linux.c
			${CMAKE_CURRENT_SOURCE_DIR}/src/file_io/file_io_linux.c
	)
	target_link_directories(platform PUBLIC
			# ${CMAKE_CURRENT_SOURCE_DIR}/libs/linux/dynamic
			${CMAKE_CURRENT_SOURCE_DIR}/libs/linux/static
	)
elseif(WIN32)
	target_compile_definitions(platform PRIVATE PLATFORM_WINDOWS)
	list(APPEND PLATFORM_SOURCES
			${CMAKE_CURRENT_SOURCE_DIR}/src/memory/memory_windows.c
			${CMAKE_CURRENT_SOURCE_DIR}/src/measure_time/measure_time_windows.c
			${CMAKE_CURRENT_SOURCE_DIR}/src/file_io/file_io_windows.c
	)
	target_link_directories(platform PUBLIC
			# ${CMAKE_CURRENT_SOURCE_DIR}/libs/windows/dynamic
			${CMAKE_CURRENT_SOURCE_DIR}/libs/windows/static
	)
endif()

target_sources(platform PRIVATE ${PLATFORM_SOURCES})

target_include_directories(platform PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/headers
		${CMAKE_SOURCE_DIR}/common/headers
)
target_link_libraries(platform
		PUBLIC
		glad
		glfw3
		common
)

if(UNIX)
	target_link_libraries(platform PUBLIC
			m
			dl
			${CMAKE_CURRENT_SOURCE_DIR}/libs/linux/static/libglfw3.a
	)
elseif(WIN32)
	target_link_libraries(platform PUBLIC
			${CMAKE_CURRENT_SOURCE_DIR}/libs/windows/static/libglfw3.a
			opengl32
			winmm
	)
endif()



add_library(glad STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/libs/src/glad/glad.c
)

target_include_directories(glad PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/headers/external
)

add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/libs/src/cglm
				# EXCLUDE_FROM_ALL
)
