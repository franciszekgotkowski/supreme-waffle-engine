#include <common/errors.h>
#include <external/cmocka.h>
#include <stdlib.h>
#include <tests/memory_arena_tests.h>
#include <engine/memory_arena.h>
#include <platform/memory_allocations.h>
#include <common/range.h>

typedef struct {
	u64 size;
	void* ptr;
} TestMemory ;

static i32 allocate_1MB_of_memory_for_arena_setup(void **state) {
	u64 memoryAmount = MB;
	TestMemory* memory = malloc(sizeof(TestMemory));
	*memory = (TestMemory){
		.size = memoryAmount,
		.ptr = malloc(memoryAmount)
	};

	*state = memory;

	return 0;
}

static i32 deallocate_allocated_memory_teardown(void **state) {
	TestMemory* memory = *state;
	free(memory->ptr);
	free(memory);

	return 0;
}

static void create_empty_memory_arena(void **state) {
	const void*  startingAddress = NULL + 1;
	const u64 capacity = 100;

	MemoryArena arena = InitializeMemoryArena(startingAddress, capacity);
	assert_true(arena.amountOfCheckpoints == 0);
	assert_true(arena.base == startingAddress);
	assert_true(arena.base == arena.top);
	assert_true(!arena.locked);
	assert_true(arena.capacity == capacity);
}

static void lock_and_unlock_arena(void **state) {
	TestMemory* memory = *state;

	MemoryArena arena = InitializeMemoryArena(memory->ptr, memory->size);
	Error err;
	void* m = registerMemory_MemoryArena(
		&arena,
		KB,
		&err
	);

	assert_false(arena.locked);
	lock_MemoryArena(&arena);
	assert_true(arena.locked);
	unlock_MemoryArena(&arena);
	assert_false(arena.locked);
}

static void register_memory_without_errors(void **state) {
	TestMemory* memory = *state;

	MemoryArena arena = InitializeMemoryArena(memory->ptr, memory->size);
	Error err;
	void* m = registerMemory_MemoryArena(
		&arena,
		KB,
		&err
	);

	assert_non_null(m);
	assert_true(err == OK);
	assert_ptr_equal(m, arena.base);
	assert_ptr_equal(arena.base + KB, arena.top);
}

static void register_memory_when_arena_is_locked(void **state) {
	TestMemory* memory = *state;

	MemoryArena arena = InitializeMemoryArena(memory->ptr, memory->size);
	Error err;
	lock_MemoryArena(&arena);

	void* m = registerMemory_MemoryArena(
		&arena,
		KB,
		&err
	);

	assert_ptr_equal(m, NULL);
	assert_true(err == LOCKED);
	assert_ptr_equal(arena.base, arena.top);
}

static void register_memory_when_there_is_no_sufficient_memory(void **state) {
	TestMemory* memory = *state;

	MemoryArena arena = InitializeMemoryArena(memory->ptr, memory->size);
	Error err;

	void* m;

	for range(i, KB){
		m = registerMemory_MemoryArena(
			&arena,
			KB,
			&err
		);
		assert_true(err == OK);
		assert_ptr_not_equal(m, NULL);
	}

	assert_ptr_equal(arena.base+memory->size, arena.top);

	for range(i, 3) {

	m = registerMemory_MemoryArena(
		&arena,
		KB,
		&err
	);
	assert_ptr_equal(m, NULL);
	assert_true(err == OUT_OF_MEMORY);
	assert_ptr_equal(arena.base+memory->size, arena.top);
	}
}

static void register_and_initialize_memory_test(void **state) {
	TestMemory* memory = *state;

	MemoryArena arena = InitializeMemoryArena(memory->ptr, memory->size);
	Error err;

	void* m;

	i32 data[] = {
		10, 2, 3, 4, 5, 6, 7, 8,
		1, 2, 3, 4, 5, 7, 8, 9
	};

	m = registerAndInitializeMemory_MemoryArena(
		&arena,
		sizeof(data),
		data,
		&err
	);
	assert_true(err == OK);
	assert_ptr_not_equal(m, NULL);
	assert_memory_equal(data, m, sizeof(data));
}

static void reset_memory_arena(void **state) {
	TestMemory* memory = *state;

	MemoryArena arena = InitializeMemoryArena(memory->ptr, memory->size);
	Error err;

	void* m;

	m = registerMemory_MemoryArena(
		&arena,
		KB,
		&err
	);
	assert_non_null(m);
	assert_true(err == OK);
	assert_ptr_equal(arena.top, arena.base + KB);
	err = reset_MemoryArena(&arena);
	assert_true(err == OK);
	assert_ptr_equal(arena.base, arena.top);


	m = registerMemory_MemoryArena(
		&arena,
		KB,
		&err
	);
	assert_non_null(m);
	assert_true(err == OK);
	assert_ptr_equal(arena.top, arena.base + KB);
	lock_MemoryArena(&arena);
	err = reset_MemoryArena(&arena);
	assert_true(err == LOCKED);
	assert_ptr_equal(arena.base + KB, arena.top);


	unlock_MemoryArena(&arena);
	err = reset_MemoryArena(&arena);
	assert_true(err == OK);
	assert_ptr_equal(arena.base, arena.top);

}


static void adding_checkpoint(void **state) {
	TestMemory* memory = *state;

	MemoryArena arena = InitializeMemoryArena(memory->ptr, memory->size);
	Error err;

	void* m;
	assert_true(0);
}

i32 run_memory_arena_test() {
	i32 success = 0;

	const struct CMUnitTest tests[]= {
		cmocka_unit_test(create_empty_memory_arena),
		cmocka_unit_test_setup_teardown(
			register_memory_without_errors,
			allocate_1MB_of_memory_for_arena_setup,
			deallocate_allocated_memory_teardown
		),
		cmocka_unit_test_setup_teardown(
			register_memory_when_arena_is_locked,
			allocate_1MB_of_memory_for_arena_setup,
			deallocate_allocated_memory_teardown
		),
		cmocka_unit_test_setup_teardown(
			register_memory_when_there_is_no_sufficient_memory,
			allocate_1MB_of_memory_for_arena_setup,
			deallocate_allocated_memory_teardown
		),
		cmocka_unit_test_setup_teardown(
			lock_and_unlock_arena,
			allocate_1MB_of_memory_for_arena_setup,
			deallocate_allocated_memory_teardown
		),
		cmocka_unit_test_setup_teardown(
			register_and_initialize_memory_test,
			allocate_1MB_of_memory_for_arena_setup,
			deallocate_allocated_memory_teardown
		),
		cmocka_unit_test_setup_teardown(
			reset_memory_arena,
			allocate_1MB_of_memory_for_arena_setup,
			deallocate_allocated_memory_teardown
		),
		cmocka_unit_test_setup_teardown(
			adding_checkpoint,
			allocate_1MB_of_memory_for_arena_setup,
			deallocate_allocated_memory_teardown
		),

		// cmocka_unit_test_setup_teardown(f, setup, teardown),
	};

	return cmocka_run_group_tests_name("Memory arena", tests, NULL, NULL);
}