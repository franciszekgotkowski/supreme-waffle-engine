#pragma once

typedef enum {
    OK,
    LOGIC_ERROR,
    OUT_OF_RANGE,
    OUT_OF_MEMORY,
    NULL_POINTER,
    SYSCALL_FAIL,
    LIBRARY_FAIL,
    LOCKED,
    FILE_NOT_FOUND,
    IO_ERROR,
    INVALID_INPUT,
    OUT_OF_INDEXES,

    AMOUNT_OF_ERRORS
} Error ;
