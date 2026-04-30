add_executable(tests)

target_sources(tests PRIVATE
		src/main.c
		src/memory_arena_tests.c
		src/resource_indexer_tests.c
		src/setup_and_teardown_functions.c
)

target_compile_options(playground PRIVATE
		$<$<CONFIG:Debug>:${DEBUG_FLAGS}>
		$<$<CONFIG:Release>:${RELEASE_FLAGS}>
)


target_include_directories(tests PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/headers

		${CMAKE_HOME_DIRECTORY}/common/headers
		${CMAKE_HOME_DIRECTORY}/platform/headers/export
		${CMAKE_HOME_DIRECTORY}/engine/headers
		${CMAKE_HOME_DIRECTORY}/playground/headers
)

target_link_directories(tests PRIVATE
		${CMAKE_HOME_DIRECTORY}/playground/libs/dynamic
		${CMAKE_HOME_DIRECTORY}/playground/libs/static

		${CMAKE_HOME_DIRECTORY}/engine/libs/static

		${CMAKE_HOME_DIRECTORY}/platform/libs/linux/static
		${CMAKE_HOME_DIRECTORY}/platform/libs/windows/static
)

add_subdirectory(cmocka)

target_link_libraries(tests PRIVATE
		engine
		platform
		common
		cmocka
)



