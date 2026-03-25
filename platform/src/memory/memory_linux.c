#include <stdio.h>
#include <sys/mman.h>

#include <export/platform/memory_allocations.h>
#include <primitives/typedefs.h>
#include <stddef.h>
#include <assert.h>

inline void* mem_alloc_on_address(u64 size, void* address) {
    // if (size % (4*KB) != 0 || size < 4*KB) {
    //     printf("extending size to fill entire page\n");
    //     u64 oldsize = size;
    //     size /= 4*KB;
    //     size *= 4*KB;
    //     size += 4*KB;
    //     printf("mapping %ld instead of %ld\n", size, oldsize);
    // }
    void* ptr = mmap(address,
    	size,
     	PROT_READ | PROT_WRITE,
      	MAP_PRIVATE | MAP_ANONYMOUS,
       	-1,
        0
    );

    assert(ptr == address);
    if (ptr != address) {
  		printf("memory allocation failed!\n");
    	return NULL;
    }

    return ptr;
}

inline void* mem_alloc(u64 size) {
    // if (size % (4*KB) != 0 || size < 4*KB) {
    //     printf("extending size to fill entire page\n");
    //     u64 oldsize = size;
    //     size /= 4*KB;
    //     size *= 4*KB;
    //     size += 4*KB;
    //     printf("mapping %ld instead of %ld\n", size, oldsize);
    // }
    void* ptr = mmap(
    	NULL,
     	size,
      	PROT_READ | PROT_WRITE,
       	MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );

    assert(ptr != MAP_FAILED);
    if (ptr == MAP_FAILED) {
  		printf("memory allocation failed!\n");
    	return NULL;
    }

    return ptr;
}

inline void mem_dealloc(void* ptr, u64 size) {
    assert(ptr != NULL);
    munmap(ptr, size);
}

