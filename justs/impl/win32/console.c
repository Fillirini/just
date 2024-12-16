#include <console.h>

#include <windows.h>


void initConsole() {
    SetConsoleOutputCP(CP_UTF8);
}



void print16(const wchar_t* cstring) {
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), cstring, wcslen(cstring), NULL, NULL);
}

void println16(const wchar_t* cstring) {
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), cstring, wcslen(cstring), NULL, NULL);
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), L"\n", 1, NULL, NULL);
}


void sprint16(const unsigned short* string, size_t length) {
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), string, length, NULL, NULL);
}

void sprintln16(const unsigned short* string, size_t length) {
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), string, length, NULL, NULL);
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), L"\n", 1, NULL, NULL);
}



void print8(const unsigned char* cstring) {
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), cstring, strlen(cstring), NULL, NULL);
}

void println8(const unsigned char* cstring) {
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), cstring, strlen(cstring), NULL, NULL);
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), u"\n", 1, NULL, NULL);
}


void sprint8(const unsigned char* string, size_t length) {
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), string, length, NULL, NULL);
}

void sprintln8(const unsigned char* string, size_t length) {
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), string, length, NULL, NULL);
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), u"\n", 1, NULL, NULL);
}