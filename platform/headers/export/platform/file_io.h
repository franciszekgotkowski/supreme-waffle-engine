#pragma once

#include <primitives/typedefs.h>
#include <primitives/errors.h>

typedef struct {
	u64 fileSize; // comes with additional \0 at the end
	void* ptr; // pointer to where file lives
} FileData ;

// reads entire file into a dynamically alocated memory region (via system calls). Gets rid of \r on endlilne on windows
// It can return:
// FILE_NOT_FOUND
// IO_ERROR
// OUT_OF_MEMORY
// OK
Error readEntireTextFile(
	str path, // in
	FileData* fileDataOut // out
);

Error freeEntireTextFile(
	FileData fileData
);

// maps file into memory
// TODO
Error mapTextFile(
	str path,
	FileData* fileData
);

Error unmapTextFile(
	FileData fileData
);