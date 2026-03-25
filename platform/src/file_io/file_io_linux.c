#include <export/platform/memory_allocations.h>
#include <primitives/errors.h>
#include <export/platform/file_io.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


Error readEntireTextFile(str path, FileData* fileData) {
	assert(path);
	assert(fileData);

	i32 fd = open(path, O_RDONLY);
	assert(fd != -1);
	if (fd == -1) {
		return FILE_NOT_FOUND;
	}

	struct stat st;
	i32 fstat_return = fstat(fd, &st);
	assert(fstat_return != -1);
	if (fstat_return == -1) {
		close(fd);
		return IO_ERROR;
	}

	assert(st.st_size > 0);
	if (st.st_size <= 0) {
		close(fd);
		return IO_ERROR;
	}

	void* data = mmap(
		NULL,
	 	st.st_size,
		PROT_READ,
		MAP_PRIVATE,
		fd,
		0
	);
	assert(data);
	if (data == NULL) {
		close(fd);
		return IO_ERROR;
	}

	FileData currentFile =  {
		.fileSize = st.st_size+1,
		.ptr = mem_alloc(currentFile.fileSize)
	};
	assert(currentFile.ptr);
	if (!currentFile.ptr) {
		return OUT_OF_MEMORY;
	}

	memcpy(currentFile.ptr, data, currentFile.fileSize);
	memcpy(currentFile.ptr+currentFile.fileSize, "\0", 1);

	*fileData =  currentFile;
	close(fd);
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