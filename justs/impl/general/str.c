#include <str.h>
#include <stdint.h>

uchar_ITER utf8Iter_next(u8StringIter* self) {
    const unsigned char *source = self->first;
    size_t dist = self->last - source; // distance to end string

    unsigned char first = *source;

    // more about UTF8 decode to Unicode : https://dev.to/emnudge/decoding-utf-8-3947

    if (first < 0x80 && dist > 0)
    {
        self->first += 1;
        return (uchar_ITER){1, first};
    }
    else if ((first & 0xE0) == 0xC0 && dist > 1)
    {
        unsigned char second = self->first[1];
        if ((second & 0xC0) != 0x80)
        {
            return (uchar_ITER){0};
        }
        self->first += 2;
        return (uchar_ITER){2, (unsigned int)(((first & 0x1F) << 6) | (second & 0x3F))};
    }
    else if ((first & 0xF0) == 0xE0 && dist > 2)
    {
        unsigned char second = self->first[1];
        unsigned char third = self->first[2];
        if ((second & 0xC0) != 0x80 || (third & 0xC0) != 0x80)
        {
            return (uchar_ITER){0};
        }
        self->first += 3;
        return (uchar_ITER){3, (unsigned int)(((first & 0x0F) << 12) | ((second & 0x3F) << 6) | (third & 0x3F))};
    }
    else if ((first & 0xF8) == 0xF0 && dist > 3)
    {
        unsigned char second = self->first[1];
        unsigned char third = self->first[2];
        unsigned char fourth = self->first[3];
        if ((second & 0xC0) != 0x80 || (third & 0xC0) != 0x80 || (fourth & 0xC0) != 0x80)
        {
            return (uchar_ITER){0};
        }
        self->first += 4;

        return (uchar_ITER){4, (unsigned int)(((first & 0x07) << 18) | ((second & 0x3F) << 12) |
                           ((third & 0x3F) << 6) | (fourth & 0x3F))};
    }
    else
    {
        return (uchar_ITER){0};
    }
}


uchar_ITER utf8Iter_peek(u8StringIter self) {
    const unsigned char *source = self.first;
    size_t dist = self.last - source; // distance to end string

    unsigned char first = *source;

    // more about UTF8 decode to Unicode : https://dev.to/emnudge/decoding-utf-8-3947

    if (first < 0x80 && dist > 0)
    {
        return (uchar_ITER){1, first};
    }
    else if ((first & 0xE0) == 0xC0 && dist > 1)
    {
        unsigned char second = self.first[1];
        if ((second & 0xC0) != 0x80)
        {
            return (uchar_ITER){0};
        }
        return (uchar_ITER){2, (unsigned int)(((first & 0x1F) << 6) | (second & 0x3F))};
    }
    else if ((first & 0xF0) == 0xE0 && dist > 2)
    {
        unsigned char second = self.first[1];
        unsigned char third = self.first[2];
        if ((second & 0xC0) != 0x80 || (third & 0xC0) != 0x80)
        {
            return (uchar_ITER){0};
        }
        return (uchar_ITER){3, (unsigned int)(((first & 0x0F) << 12) | ((second & 0x3F) << 6) | (third & 0x3F))};
    }
    else if ((first & 0xF8) == 0xF0 && dist > 3)
    {
        unsigned char second = self.first[1];
        unsigned char third = self.first[2];
        unsigned char fourth = self.first[3];
        if ((second & 0xC0) != 0x80 || (third & 0xC0) != 0x80 || (fourth & 0xC0) != 0x80)
        {
            return (uchar_ITER){0};
        }
        return (uchar_ITER){4, (unsigned int)(((first & 0x07) << 18) | ((second & 0x3F) << 12) |
                           ((third & 0x3F) << 6) | (fourth & 0x3F))};
    }
    else
    {
        return (uchar_ITER){0};
    }
}