#pragma once

#include <common/typedefs.h>

i32 allocate_1MB_of_memory_for_arena_setup(void **state);

i32 deallocate_allocated_memory_teardown(void **state);