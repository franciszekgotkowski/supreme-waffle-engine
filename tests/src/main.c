#include <common/typedefs.h>
#include <stdio.h>

#include <tests/memory_arena_tests.h>

int main(void) {
	i32 success = 0;

    success += run_memory_arena_test();

    if (success == 0) {
    	printf("\033[32m\n0================================0\n");
    	printf("All tests passed!");
    	printf("\n0================================0\n\033[0m");
    } else {
    	printf("\033[31m\n0================================0\n");
   		printf("%d test groups did not pass!", success);
    	printf("\n0================================0\n\033[0m");
    }

    return success;
}
