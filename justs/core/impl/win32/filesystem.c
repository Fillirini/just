#include <src/filesystem.h>

#include <windows.h>

bool readFile(unsigned short* fPath, FileData* pFile) {
    DWORD fAttr = GetFileAttributesW(fPath);

    BOOL fExists = (fAttr != INVALID_FILE_ATTRIBUTES && !(fAttr & FILE_ATTRIBUTE_DIRECTORY));

    if (fExists) {
        HANDLE hFile = CreateFileW(fPath, GENERIC_READ, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        if (hFile != INVALID_HANDLE_VALUE) {
            DWORD fSize = GetFileSize(hFile, 0);
            unsigned char* buf = (unsigned char*)malloc(fSize);
            if (ReadFile(hFile, buf, fSize, NULL, NULL)) {
                *pFile = (FileData){buf, fSize};
                return true;
            }
        }
    }

    return false;
}