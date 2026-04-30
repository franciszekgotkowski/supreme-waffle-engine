#include <tests/setup_and_teardown_functions.h>
#include <tests/test_memory_struct.h>
#include <platform/memory_allocations.h>
#include <common/typedefs.h>
#include <stdlib.h>

i32 allocate_1MB_of_memory_for_arena_setup(void **state) {
	u64 memoryAmount = MB;
	TestMemory* memory = malloc(sizeof(TestMemory));
	*memory = (TestMemory){
		.size = memoryAmount,
		.ptr = malloc(memoryAmount)
	};

	*state = memory;

	return 0;
}

i32 deallocate_allocated_memory_teardown(void **state) {
	TestMemory* memory = *state;
	free(memory->ptr);
	free(memory);

	return 0;
}
