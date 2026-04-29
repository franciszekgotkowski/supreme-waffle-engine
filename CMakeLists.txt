cmake_minimum_required(VERSION 3.10)

project(MyGame C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CONFIGURATION_TYPES
				Debug
				Release
				# Benchmark
				# DebugBenchmark
)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
# set(CMAKE_C_FLAGS_DEBUG "-g -O0 -pg")
# set(CMAKE_C_FLAGS_RELEASE "-O3 -Wno-unused-but-set-variable -Wno-unused-variable")

set(COMMON_FLAGS -Wall -Werror)
set(DEBUG_FLAGS -g -O0)
#set(DEBUG_FLAGS -g -O0 -pg)
set(RELEASE_FLAGS -O3 -Wno-unused-but-set-variable -Wno-unused-variable)

set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)


set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(common)
add_subdirectory(platform)
add_subdirectory(engine)
add_subdirectory(playground)
add_subdirectory(tests)
