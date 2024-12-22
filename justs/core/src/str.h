#ifndef STR_H
#define STR_H

#include <stdint.h>
#include <stdbool.h>

typedef struct u8String {
    unsigned char* data;
    size_t length;
    size_t capacity;
} u8String;

typedef struct u8Slice {
    unsigned char* data;
    size_t length;
} u8Slice;

bool u8Slice_isEquals(u8Slice s1, u8Slice s2);

typedef struct u16String {
    unsigned short* data;
    size_t length;
    size_t capacity;
    //bool hasDoubleChar;
} u16String;

typedef struct u16Slice {
    unsigned char* data;
    size_t length;
} u16Slice;



typedef struct uchar_ITER {
    size_t size;
    unsigned int symbol;
} uchar_ITER;

typedef struct u8StringIter {
    unsigned char* first;
    unsigned char* last;
} u8StringIter;

uchar_ITER utf8Iter_next(u8StringIter* self);
uchar_ITER utf8Iter_peek(u8StringIter self);
uchar_ITER utf8Iter_npeek(u8StringIter self, size_t offset);

#endif // STR_H