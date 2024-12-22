#ifndef CONSOLE_H
#define CONSOLE_H

#include <src/str.h>

// output utf16 string

void initConsole();

void cprint16(const unsigned short* cstring);
void cprintln16(const unsigned short* cstring);

void nprint16(const unsigned short* string, size_t length);
void nprintln16(const unsigned short* string, size_t length);

void sprint16(u16Slice slice);
void sprintln16(u16Slice slice);

// output utf8 string

void cprint8(const unsigned char* cstring);
void cprintln8(const unsigned char* cstring);

void nprint8(const unsigned char* string, size_t length);
void nprintln8(const unsigned char* string, size_t length);

void sprint8(u8Slice slice);
void sprintln8(u8Slice slice);

#endif // CONSOLE_H