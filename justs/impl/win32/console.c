#include <console.h>

#include <windows.h>

void print(const wchar_t* cstring) {
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), cstring, wcslen(cstring), 0, 0);
}
void println(const wchar_t* cstring) {
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), cstring, wcslen(cstring), 0, 0);
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), "\n", 1, 0, 0);
}