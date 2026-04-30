#include <common/errors.h>
#include <common/memory_arena.h>
#include <engine/resource_indexers.h>
#include <tests/test_memory_struct.h>
#include <external/cmocka.h>
#include <tests/resource_indexer_tests.h>
#include <tests/setup_and_teardown_functions.h>
#include <platform/memory_allocations.h>

static void create_empty_indexer(void **state) {

	TestMemory* memory = *state;
	MemoryArena arena = InitializeMemoryArena(
		memory->ptr,
		memory->size
	);

	const u32 amountOfResources = 10;

	Error err;
	ResourceIndexer* indexer = registerMemory_MemoryArena(
		&arena,
		GetWholeIndexerSize(amountOfResources),
		&err
	);

	InitializeResourceIndexer(
		indexer,
		amountOfResources,
		&arena
	);

	assert_true(indexer->maxAmountOfResources == amountOfResources);
	assert_true(indexer->currentAmountOfResources == 0);
	assert_true(indexer->indexes == (void*)indexer + sizeof(ResourceIndexer));
	assert_true(indexer->arena == &arena);

}

static void create_empty_indexer_into_arena(void **state) {

	TestMemory* memory = *state;
	MemoryArena arena = InitializeMemoryArena(
		memory->ptr,
		memory->size
	);

	const u32 amountOfResources = 10;

	Error err;
	ResourceIndexer* indexer = InitializeResourceIntoArena(
		&arena,
		amountOfResources,
		&err
	);
	assert_true(err == OK);

	assert_true(indexer->maxAmountOfResources == amountOfResources);
	assert_true(indexer->currentAmountOfResources == 0);
	assert_true(indexer->indexes == (void*)indexer + sizeof(ResourceIndexer));
	assert_true(indexer->arena == &arena);

}

static void create_empty_indexer_into_locked_arena(void **state) {

	TestMemory* memory = *state;
	MemoryArena arena = InitializeMemoryArena(
		memory->ptr,
		memory->size
	);
	lock_MemoryArena(&arena);

	const u32 amountOfResources = 10;

	Error err;
	ResourceIndexer* indexer = InitializeResourceIntoArena(
		&arena,
		amountOfResources,
		&err
	);
	assert_true(err == LOCKED);

	assert_ptr_equal(indexer, NULL);

}

static void create_empty_indexer_into_arena_with_no_memory_left(void **state) {
	const u32 amountOfResources = 10;

	TestMemory* memory = *state;
	MemoryArena arena = InitializeMemoryArena(
		memory->ptr,
		GetWholeIndexerSize(amountOfResources)*2
	);

	Error err;
	ResourceIndexer* indexer = InitializeResourceIntoArena(
		&arena,
		amountOfResources,
		&err
	);
	assert_true(err == OK);
	assert_ptr_not_equal(indexer, NULL);

	indexer = InitializeResourceIntoArena(
		&arena,
		amountOfResources,
		&err
	);
	assert_true(err == OK);
	assert_ptr_not_equal(indexer, NULL);

	indexer = InitializeResourceIntoArena(
		&arena,
		amountOfResources,
		&err
	);
	assert_true(err == OUT_OF_MEMORY);
	assert_ptr_equal(indexer, NULL);

}

static void register_resource_with_insufficient_memory_in_arena(void **state) {
	const u32 amountOfResources = 10;

	TestMemory* memory = *state;
	MemoryArena arena = InitializeMemoryArena(
		memory->ptr,
		GetWholeIndexerSize(amountOfResources) + KB
	);

	Error err;
	ResourceIndexer* indexer = InitializeResourceIntoArena(
		&arena,
		amountOfResources,
		&err
	);
	assert_true(err == OK);
	assert_ptr_not_equal(indexer, NULL);

	ID id = RegisterNewResource_ResourceIndexer(
		indexer,
		KB/2,
		&err
	);
	assert_true(err == OK);
	assert_int_not_equal(id, -1);

	id = RegisterNewResource_ResourceIndexer(
		indexer,
		KB/2,
		&err
	);
	assert_true(err == OK);
	assert_int_not_equal(id, -1);

	id = RegisterNewResource_ResourceIndexer(
		indexer,
		KB/2,
		&err
	);
	assert_true(err == OUT_OF_MEMORY);
	assert_int_equal(id, -1);
}

static void register_resource_with_insufficient_amount_of_indexes(void **state) {
	const u32 amountOfResources = 2;

	TestMemory* memory = *state;
	MemoryArena arena = InitializeMemoryArena(
		memory->ptr,
		memory->size
	);

	Error err;
	ResourceIndexer* indexer = InitializeResourceIntoArena(
		&arena,
		amountOfResources,
		&err
	);
	assert_true(err == OK);
	assert_ptr_not_equal(indexer, NULL);

	ID id = RegisterNewResource_ResourceIndexer(
		indexer,
		KB/2,
		&err
	);
	assert_true(err == OK);
	assert_int_not_equal(id, -1);

	id = RegisterNewResource_ResourceIndexer(
		indexer,
		KB/2,
		&err
	);
	assert_true(err == OK);
	assert_int_not_equal(id, -1);

	id = RegisterNewResource_ResourceIndexer(
		indexer,
		KB/2,
		&err
	);
	assert_true(err == OUT_OF_INDEXES);
	assert_int_equal(id, -1);

	id = RegisterNewResource_ResourceIndexer(
		indexer,
		KB/2,
		&err
	);
	assert_true(err == OUT_OF_INDEXES);
	assert_int_equal(id, -1);
}

static void register_resource_when_arena_is_locked(void **state) {
	const u32 amountOfResources = 10;

	TestMemory* memory = *state;
	MemoryArena arena = InitializeMemoryArena(
		memory->ptr,
		memory->size
	);

	Error err;
	ResourceIndexer* indexer = InitializeResourceIntoArena(
		&arena,
		amountOfResources,
		&err
	);
	assert_true(err == OK);
	assert_ptr_not_equal(indexer, NULL);

	ID id = RegisterNewResource_ResourceIndexer(
		indexer,
		KB/2,
		&err
	);
	assert_true(err == OK);
	assert_int_not_equal(id, -1);

	lock_MemoryArena(&arena);
	id = RegisterNewResource_ResourceIndexer(
		indexer,
		KB/2,
		&err
	);
	assert_true(err == LOCKED);
	assert_int_equal(id, -1);

	unlock_MemoryArena(&arena);
	id = RegisterNewResource_ResourceIndexer(
		indexer,
		KB/2,
		&err
	);
	assert_true(err == OK);
	assert_int_not_equal(id, -1);
}

static void get_existing_resource(void **state) {
	const u32 amountOfResources = 10;

	TestMemory* memory = *state;
	MemoryArena arena = InitializeMemoryArena(
		memory->ptr,
		memory->size
	);

	Error err;
	ResourceIndexer* indexer = InitializeResourceIntoArena(
		&arena,
		amountOfResources,
		&err
	);
	assert_true(err == OK);
	assert_ptr_not_equal(indexer, NULL);

	ID id;

	id = RegisterNewResource_ResourceIndexer(
		indexer,
		KB/2,
		&err
	);
	assert_true(err == OK);
	assert_int_not_equal(id, -1);

	id = RegisterNewResource_ResourceIndexer(
		indexer,
		KB/2,
		&err
	);
	assert_true(err == OK);
	assert_int_not_equal(id, -1);

	void* ptr;
	ptr = GetResource_ResourceIndexer(
		indexer,
		0,
		&err
	);
	assert_true(err == OK);
	assert_ptr_not_equal(ptr, NULL);

	GetResource_ResourceIndexer(
		indexer,
		1,
		&err
	);
	assert_true(err == OK);
	assert_ptr_not_equal(ptr, NULL);

}

static void get_not_existing_resource(void **state) {
	const u32 amountOfResources = 10;

	TestMemory* memory = *state;
	MemoryArena arena = InitializeMemoryArena(
		memory->ptr,
		memory->size
	);

	Error err;
	ResourceIndexer* indexer = InitializeResourceIntoArena(
		&arena,
		amountOfResources,
		&err
	);
	assert_true(err == OK);
	assert_ptr_not_equal(indexer, NULL);

	ID id;
	void* ptr;

	ptr = GetResource_ResourceIndexer(
		indexer,
		0,
		&err
	);
	assert_true(err == OUT_OF_RANGE);
	assert_ptr_equal(ptr, NULL);

	ptr = GetResource_ResourceIndexer(
		indexer,
		-1,
		&err
	);
	assert_true(err == OUT_OF_RANGE);
	assert_ptr_equal(ptr, NULL);

	id = RegisterNewResource_ResourceIndexer(
		indexer,
		KB/2,
		&err
	);
	assert_true(err == OK);
	assert_int_not_equal(id, -1);

	id = RegisterNewResource_ResourceIndexer(
		indexer,
		KB/2,
		&err
	);
	assert_true(err == OK);
	assert_int_not_equal(id, -1);

	ptr = GetResource_ResourceIndexer(
		indexer,
		-1,
		&err
	);
	assert_true(err == OUT_OF_RANGE);
	assert_ptr_equal(ptr, NULL);

	ptr = GetResource_ResourceIndexer(
		indexer,
		0,
		&err
	);
	assert_true(err == OK);
	assert_ptr_not_equal(ptr, NULL);

	GetResource_ResourceIndexer(
		indexer,
		1,
		&err
	);
	assert_true(err == OK);
	assert_ptr_not_equal(ptr, NULL);

	ptr = GetResource_ResourceIndexer(
		indexer,
		-1,
		&err
	);
	assert_true(err == OUT_OF_RANGE);
	assert_ptr_equal(ptr, NULL);

	ptr = GetResource_ResourceIndexer(
		indexer,
		0,
		&err
	);
	assert_true(err == OK);
	assert_ptr_not_equal(ptr, NULL);

	ptr = GetResource_ResourceIndexer(
		indexer,
		1,
		&err
	);
	assert_true(err == OK);
	assert_ptr_not_equal(ptr, NULL);

	ptr = GetResource_ResourceIndexer(
		indexer,
		2,
		&err
	);
	assert_true(err == OUT_OF_RANGE);
	assert_ptr_equal(ptr, NULL);

}

i32 run_resource_indexer_tests() {
	i32 success = 0;

	const struct CMUnitTest tests[]= {
		cmocka_unit_test_setup_teardown(
			create_empty_indexer,
			allocate_1MB_of_memory_for_arena_setup,
			deallocate_allocated_memory_teardown
		),
		cmocka_unit_test_setup_teardown(
			create_empty_indexer_into_arena,
			allocate_1MB_of_memory_for_arena_setup,
			deallocate_allocated_memory_teardown
		),
		cmocka_unit_test_setup_teardown(
			create_empty_indexer_into_locked_arena,
			allocate_1MB_of_memory_for_arena_setup,
			deallocate_allocated_memory_teardown
		),
		cmocka_unit_test_setup_teardown(
			create_empty_indexer_into_arena_with_no_memory_left,
			allocate_1MB_of_memory_for_arena_setup,
			deallocate_allocated_memory_teardown
		),
		cmocka_unit_test_setup_teardown(
			register_resource_with_insufficient_memory_in_arena,
			allocate_1MB_of_memory_for_arena_setup,
			deallocate_allocated_memory_teardown
		),
		cmocka_unit_test_setup_teardown(
			register_resource_with_insufficient_amount_of_indexes,
			allocate_1MB_of_memory_for_arena_setup,
			deallocate_allocated_memory_teardown
		),
		cmocka_unit_test_setup_teardown(
			register_resource_when_arena_is_locked,
			allocate_1MB_of_memory_for_arena_setup,
			deallocate_allocated_memory_teardown
		),
		cmocka_unit_test_setup_teardown(
			get_existing_resource,
			allocate_1MB_of_memory_for_arena_setup,
			deallocate_allocated_memory_teardown
		),
		cmocka_unit_test_setup_teardown(
			get_not_existing_resource,
			allocate_1MB_of_memory_for_arena_setup,
			deallocate_allocated_memory_teardown
		),

	};

	return cmocka_run_group_tests_name("Resource Indexer", tests, NULL, NULL);
}
