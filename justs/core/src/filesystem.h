#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <stdbool.h>
#include <stdint.h>

typedef struct FileData {
    unsigned char* data;
    size_t size; 
} FileData;

bool readFile(unsigned short* path, FileData* pFile);

#endif // FILE_SYSTEM_H