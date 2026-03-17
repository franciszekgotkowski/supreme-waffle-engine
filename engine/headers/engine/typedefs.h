#pragma once

#include <stdint.h>

#define nullptr (void*)0

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef const char* str;
typedef char* mstr;

typedef long long unsigned int llu;

typedef union {
    u32 arr[2];
    struct {
        u32 x;
        u32 y;
    };
} v2u32;

typedef union {
    i32 arr[2];
    struct {
        i32 x;
        i32 y;
    };
} v2i32;

typedef union {
    f32 arr[2];
    struct {
        f32 x;
        f32 y;
    };
} v2;

typedef union {
    f32 arr[3];
    struct {
        f32 x;
        f32 y;
        f32 z;
    };
} v3;

typedef union {
    f32 arr[4];
    struct {
        f32 x;
        f32 y;
        f32 z;
        f32 w;
    };
} v4;

#ifndef NDEBUG
	#define DEBUG_ONLY(CONTENT) CONTENT;
#else
	#define DEBUG_ONLY(CONTENT);
#endif

#define NOT_COMPLETED 0