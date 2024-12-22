#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

unsigned int toUpperCase(unsigned int uint);

unsigned int toLowerCase(unsigned int uint);

bool isAlpha(unsigned int uint);

bool isDigit(unsigned int uint, unsigned char radix);

bool isHexDigit(unsigned int uint);

bool isDecimalDigit(unsigned int uint);

bool isOctalDigit(unsigned int uint);

bool isBinaryDigit(unsigned int uint);

bool isLowerCase(unsigned int uint);

bool isUpperCase(unsigned int uint);

bool isWhitespace(unsigned int uint);

bool isControlChar(unsigned int uint);
