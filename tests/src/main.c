#include <external/cmocka.h>
#include <common/typedefs.h>

#include <tests/memory_arena_tests.h>

int main(void) {
	u32 success = 0;

    success += run_memory_arena_test();

    return success;
}