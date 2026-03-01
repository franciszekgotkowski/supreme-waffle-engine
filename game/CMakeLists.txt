add_executable(game)

target_sources(game PRIVATE
	src/main.c
	src/game_loop.c
	src/input_functions.c
)

target_include_directories(game PRIVATE
	${CMAKE_HOME_DIRECTORY}/engine/headers
	${CMAKE_CURRENT_SOURCE_DIR}/headers
)

target_link_directories(game PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/libs/dynamic
	${CMAKE_CURRENT_SOURCE_DIR}/libs/static
	${CMAKE_HOME_DIRECTORY}/engine/libs/static
)


target_link_libraries(game
	PRIVATE engine
		SDL3
)

