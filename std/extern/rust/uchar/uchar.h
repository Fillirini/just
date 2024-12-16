#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

unsigned int toUpperCase(unsigned int uchar);

unsigned int toLowerCase(unsigned int uchar);

bool isAlpha(unsigned int uchar);

bool isDigit(unsigned int uchar, unsigned char radix);

bool isHexDigit(unsigned int uchar);

bool isDecimalDigit(unsigned int uchar);

bool isOctalDigit(unsigned int uchar);

bool isBinaryDigit(unsigned int uchar);

bool isLowerCase(unsigned int uchar);

bool isUpperCase(unsigned int uchar);

bool isWhitespace(unsigned int uchar);

bool isControlChar(unsigned int uchar);
