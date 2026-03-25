#include <assert.h>
#include <fileapi.h>
#include <windows.h>
#include <stdbool.h>

#include <engine/platform/file_io.h>
#include <engine/platform/memory_allocations.h>
#include <engine/range.h>

// Reads file string, and copies its insides but removes \r\n from file and leaves just \n
// Returns FileData with data under .ptr pointer
static FileData copyAndPrepare(str file, i64 fileSize) {
    assert(fileSize >= 0);
    assert(file);

    u64 amountOfr = 0;
    char r = '\r';
    char n = '\n';
    for range(i, fileSize) {
        if (file[i] == r) {
            amountOfr++;
        }
    }

    FileData returnData = {
        .fileSize = (u64) fileSize - amountOfr,
        .ptr = mem_alloc((u64) fileSize - amountOfr)
    };
    assert(returnData.ptr);

    u64 returnFileCharIdx = 0;
    for range(i, fileSize) {
        if (i < fileSize - 1) {
            if (file[i] == r && file[i + 1] == n) {
                ((char *) returnData.ptr)[returnFileCharIdx] = '\n';
                i++;
            } else {
                ((char *) returnData.ptr)[returnFileCharIdx] = file[i];
            }
        }
        returnFileCharIdx++;
    }

    assert(returnFileCharIdx == returnData.fileSize);

    return returnData;
}

Error readEntireTextFile(str path, FileData *fileDataOut) {
    assert(path);
    assert(fileDataOut);
    assert(fileDataOut->ptr);
    assert(fileDataOut->fileSize > 0);

    HANDLE fileHandle = CreateFileA(
        path,
        GENERIC_READ,
        FILE_SHARE_READ,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0
    );

    assert((u64)fileHandle != ERROR_FILE_NOT_FOUND);
    if ((u64) fileHandle == ERROR_FILE_NOT_FOUND) {
        return FILE_NOT_FOUND;
    }

    LARGE_INTEGER fileSizeTemp; // LARGE_INTEGER is 64 bit signed integer
    GetFileSizeEx(fileHandle, &fileSizeTemp);

    i64 fileSize = fileSizeTemp.QuadPart;
    assert(fileSize >= 0);
    if (fileSize < 0) {
        CloseHandle(fileHandle);
        return IO_ERROR;
    }

    void* tempFileData = mem_alloc(fileSize+1);
    ((char*)tempFileData)[fileSize] = '\0';
    bool fileRead = ReadFile(
        fileHandle,
        tempFileData,
        fileSize,
        0,
        0
    );
    assert(fileRead);
    if (fileRead == 0) {
        mem_dealloc(tempFileData, fileSize+1);
        CloseHandle(fileHandle);
        return IO_ERROR;
    }

    FileData cleanFile = copyAndPrepare(tempFileData, fileSize+1);
    assert(cleanFile.ptr);
    assert(cleanFile.fileSize > 0);

    mem_dealloc(tempFileData, fileSize+1);
    CloseHandle(fileHandle);
    *fileDataOut = cleanFile;
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
