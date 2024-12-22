#include <src/console.h>

#include <windows.h>


void initConsole() {
    SetConsoleOutputCP(CP_UTF8);
}



void cprint16(const unsigned short* cstring) {
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), cstring, wcslen(cstring), NULL, NULL);
}

void cprintln16(const unsigned short* cstring) {
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), cstring, wcslen(cstring), NULL, NULL);
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), L"\n", 1, NULL, NULL);
}


void nprint16(const unsigned short* string, size_t length) {
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), string, length, NULL, NULL);
}

void nprintln16(const unsigned short* string, size_t length) {
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), string, length, NULL, NULL);
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), L"\n", 1, NULL, NULL);
}



void cprint8(const unsigned char* cstring) {
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), cstring, strlen(cstring), NULL, NULL);
}

void cprintln8(const unsigned char* cstring) {
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), cstring, strlen(cstring), NULL, NULL);
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), u"\n", 1, NULL, NULL);
}


void nprint8(const unsigned char* string, size_t length) {
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), string, length, NULL, NULL);
}

void nprintln8(const unsigned char* string, size_t length) {
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), string, length, NULL, NULL);
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), u"\n", 1, NULL, NULL);
}