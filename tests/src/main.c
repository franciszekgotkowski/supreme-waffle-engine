#include <common/typedefs.h>
#include <stdio.h>

#include <tests/memory_arena_tests.h>

int main(void) {
	printf("jlak");
	i32 success = 0;

    success += run_memory_arena_test();
	printf("jlak");

    return success;
}