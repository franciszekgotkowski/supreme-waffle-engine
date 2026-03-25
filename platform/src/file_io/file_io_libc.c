
// DEPRECATED
// PLEASE DONT USE IT
// FILE IS SAVED FOR THE SAKE OF PRESERVATION

#include <engine/errors.h>
#include <engine/platform/memory_allocations.h>
#include <assert.h>
#include <stdio.h>

#include <engine/platform/file_io.h>

Error readEntireTextFile(str path, FileData* returnFileData) {
	assert(path);
	assert(returnFileData);

	FILE* file = fopen(path, "r");
	assert(file);
	if (!file) {
		return FILE_NOT_FOUND;
	}

	fseek(file, 0, SEEK_END);
	u64 fileSize = ftell(file);
	rewind(file);

	FileData currentFile = {
		.fileSize = fileSize+1,
		.ptr = mem_alloc(currentFile.fileSize),
	};
	assert(currentFile.ptr);
	if (!currentFile.ptr) {
		return OUT_OF_MEMORY;
	}

	u64 readSize = fread(currentFile.ptr, 1, fileSize, file);
	assert(readSize == fileSize);
	if (readSize != fileSize) {
		mem_dealloc(currentFile.ptr, currentFile.fileSize);
		return IO_ERROR;
	}

	((char*)(currentFile.ptr))[currentFile.fileSize-1] = '\0';
	*returnFileData = currentFile;
	fclose(file);

	return OK;
}

Error freeEntireTextFile(FileData fileData) {
	assert(fileData.ptr);
	if (!fileData.ptr) {
		return NULL_POINTER;
	}

	mem_dealloc(fileData.ptr, fileData.fileSize);

	return OK;
}