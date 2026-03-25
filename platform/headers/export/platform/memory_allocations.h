#pragma once

#include <primitives/typedefs.h>

#define KB ((u64)1024)
#define MB ((u64)KB*1024)
#define GB ((u64)MB*1024)

#define BASE_POOL_ADDRESS 0x600000000000

// allocates memory via system calls
void* mem_alloc(u64 size);
void* mem_alloc_on_address(u64 size, void* address);
void mem_dealloc(void* ptr, u64 size);

