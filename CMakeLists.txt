cmake_minimum_required(VERSION 3.10)

project(MyGame C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CONFIGURATION_TYPES
				Debug
				Release
				# Benchmark
				# DebugBenchmark
				# CACHE STRING "Available build types"
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
set(CMAKE_C_FLAGS_RELEASE "-O3 -Wno-unused-but-set-variable -Wno-unused-variable")

set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)

# if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
#     set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type")
# endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(ENGINE_ENABLE_ASSERTS "Enable engine asserts" ON)
option(GAME_ENABLE_ASSERTS "Enable engine asserts" ON)

add_subdirectory(engine)
add_subdirectory(game)
# add_subdirectory(utils)
# add_subdirectory(tests)
