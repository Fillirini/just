#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h> // bool
#include <stdint.h> // size_t

#include <src/str.h> // u8Slice

typedef struct TextPos {
    size_t index;
    size_t line;
    size_t column;
} TextPos;

typedef struct Token {
    size_t kind;
    u8Slice content;
    TextPos pos;
} Token;

typedef struct TokError {
    size_t kind;
    TextPos pos;
    
    // loophole if standard fields are not enough to report an error
    void* extension; 
} TokError;

typedef struct TokResult {
    bool isSuccess;
    union {
        Token token;
        TokError error;
    } value;
} TokResult, TokenizerResult;

#endif // TOKEN_H