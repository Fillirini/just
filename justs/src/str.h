#ifndef STR_H
#define STR_H

typedef struct uchar_ITER {
    size_t size;
    unsigned int symbol;
} uchar_ITER;

typedef struct StringIter {
    unsigned char* first;
    unsigned char* last;
} StringIter;

uchar_ITER utf8Iter_next(StringIter* self);
uchar_ITER utf8Iter_peek(StringIter self);

#endif // STR_H