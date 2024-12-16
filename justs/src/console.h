#ifndef CONSOLE_H
#define CONSOLE_H

// output utf16 string

void initConsole();

void print16(const unsigned short* cstring);
void println16(const unsigned short* cstring);

void sprint16(const unsigned short* string, size_t length);
void sprintln16(const unsigned short* string, size_t length);

// output utf8 string

void print8(const unsigned char* cstring);
void println8(const unsigned char* cstring);

void sprint8(const unsigned char* string, size_t length);
void sprintln8(const unsigned char* string, size_t length);

#endif // CONSOLE_H