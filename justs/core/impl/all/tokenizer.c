#include <src/tokenizer.h>
#include <deps/uchar/uchar.h>


// пропуск пробелов. изменяет состояние токенизатора.
// увеличивает счетчик индекса, колонки.
// учитывает символ перехода на новую строку.
void TokenizerState_skipWhitespace(TokenizerState* self);
// передвинуть указатели индекса и колонки.
// параметр `len` - длина символа UTF8 в байтах 
void TokenizerState_acceptChar(TokenizerState* self, uint32_t uchar, uint8_t len);
void TokenizerState_backChar(TokenizerState* self, uint8_t len);
TextPos TokenizerState_getPos(TokenizerState* self);
u8Slice TokenizerState_getSlice(TokenizerState* self);

TokResult TokenizerState_acceptIdent(TokenizerState* self);
TokResult TokenizerState_acceptNumber(TokenizerState* self);
TokResult TokenizerState_acceptOperator(TokenizerState* self);
TokResult TokenizerState_acceptComment(TokenizerState* self);
TokResult TokenizerState_acceptString(TokenizerState* self);

/*========================HELPERS===========================*/

TextPos TokenizerState_getPos(TokenizerState* self) {
    return (TextPos){
        .index=self->startIndex,
        .line=self->curLine,
        .column=self->startColumn
    };
}

u8Slice TokenizerState_getSlice(TokenizerState* self) {
    return (u8Slice){
        .data = &self->source.data[self->startIndex],
        .length = self->curIndex - self->startIndex
    };
}

void TokenizerState_acceptChar(TokenizerState* self, uint32_t uchar, uint8_t len) {
    self->iter.first += len;
    self->curIndex += len;
    if (uchar == '\n') { 
        self->curColumn = 0;
        self->curLine += 1;
    } else {
        self->curColumn += 1;
    }
}

void TokenizerState_backChar(TokenizerState* self, uint8_t len) {
    self->iter.first -= len;
    self->curIndex -= len;
    self->curColumn -= 1;
}

void TokenizerState_skipWhitespace(TokenizerState* self) {
    uchar_ITER optionChar = utf8Iter_peek(self->iter);

    while (optionChar.size != 0)
    {
        if (isWhitespace(optionChar.symbol)) {
            TokenizerState_acceptChar(self, optionChar.symbol, optionChar.size);
        } else {
            break;
        }

        optionChar = utf8Iter_peek(self->iter);
    }

    self->startIndex = self->curIndex;
    self->startColumn = self->curColumn;
}

/*=====================ENTRY POINT========================*/

TokResult TokenizerState_next(TokenizerState* self) {
    // пропуск пробелов, пока не встретим какй-то другой тип символов
    TokenizerState_skipWhitespace(self);

    uchar_ITER optionChar = utf8Iter_peek(self->iter);

    if (optionChar.size == 0) {
        //End of File.
        return (TokenizerResult){
            .isSuccess=true,
            .value=(Token){
                .kind=EoF,
                .content=TokenizerState_getSlice(self),
                .pos=TokenizerState_getPos(self)
            }
        };
    }

    uint32_t uchar = optionChar.symbol;
    if (isAlpha(uchar) || uchar == '_') {
        // идентификатор
        return TokenizerState_acceptIdent(self);
    } else if (uchar == '.' || isDecimalDigit(uchar)) {
        // число или точка
        return TokenizerState_acceptNumber(self);
    } else if (uchar == '/') {
        return TokenizerState_acceptComment(self);
    } else if (uchar == '"') {
        return TokenizerState_acceptString(self);
    } else {
        return TokenizerState_acceptOperator(self);
    }
}

/*=====================TOKENIZERS========================*/

/* TOKENIZE IDENT */
TokResult TokenizerState_acceptIdent(TokenizerState* self) {
    size_t kindToken = Ident;

    uchar_ITER optionChar = utf8Iter_peek(self->iter);

    if (optionChar.size != 0) {
        if (isAlpha(optionChar.symbol) || optionChar.symbol == '_') { 
            TokenizerState_acceptChar(self, optionChar.symbol, optionChar.size);
            optionChar = utf8Iter_peek(self->iter);
        }
        else { // ERROR! it's not IDENT!
            return (TokenizerResult){
                .isSuccess=false,
                .value.error = (TokError){
                    .kind=ItsNotIdent,
                    .pos=TokenizerState_getPos(self)
                }
            };
        }
    }

    while (optionChar.size != 0)
    {
        uint32_t uchar = optionChar.symbol;
        
        if (isAlpha(uchar) || isDecimalDigit(uchar) || uchar == '_') {
            TokenizerState_acceptChar(self, uchar, optionChar.size);
        } else {
            // it's prefix, f.e. `u16`, `u8`
            if (uchar == '"') {
                u8Slice prefix = TokenizerState_getSlice(self);
                if (prefix.length == 1 && (*prefix.data == 'f' || *prefix.data == 'F')) {
                    // format string
                }

                kindToken = PrefixString;
            }

            break;
        }

        optionChar = utf8Iter_peek(self->iter);
    }
    
    Token token = (Token){
        .kind=kindToken,
        .content=TokenizerState_getSlice(self),
        .pos=TokenizerState_getPos(self)
    };

    return (TokResult){
        .isSuccess=true,
        .value.token=token
    };
}

/* TOKENIZE NUMBER */
TokResult TokenizerState_acceptNumber(TokenizerState* self) {
    size_t kindToken = IntNumber;

    uchar_ITER optionChar = utf8Iter_peek(self->iter);
    
    // it's not start of number
    if (optionChar.size == 0 || !(isDecimalDigit(optionChar.symbol) || optionChar.symbol == '.')) {
        return (TokenizerResult){
            .isSuccess=false,
            .value.error = (TokError){
                .kind=ItsNotNumber,
                .pos=TokenizerState_getPos(self)
            }
        };
    }

    // it's decimal separator or dot (access to struct member)
    if (optionChar.symbol == '.') {
        TokenizerState_acceptChar(self, optionChar.symbol, optionChar.size);
        optionChar = utf8Iter_peek(self->iter);

        if (!isDecimalDigit(optionChar.symbol)) {
            // it's dot, not float-number     
            return (TokResult){
                .isSuccess=true,
                .value.token=(Token){
                    .kind=Dot,
                    .content=TokenizerState_getSlice(self),
                    .pos=TokenizerState_getPos(self)
                }
            };

        } else {
            kindToken = FloatNumber;
        }

    } else if (optionChar.symbol == '0') { // maybe it's not decimal number
        TokenizerState_acceptChar(self, optionChar.symbol, optionChar.size);
        optionChar = utf8Iter_peek(self->iter);

        if (optionChar.symbol == 'x' || optionChar.symbol == 'X') {
            // it's hex number
            kindToken = HexNumber;

            TokenizerState_acceptChar(self, optionChar.symbol, optionChar.size);
            optionChar = utf8Iter_peek(self->iter);

            while (optionChar.size != 0)
            {
                uint32_t uchar = optionChar.symbol;

                if (isHexDigit(uchar) || uchar == '_') {
                    TokenizerState_acceptChar(self, uchar, optionChar.size);
                } else {
                    break;
                }

                optionChar = utf8Iter_peek(self->iter);
            }

        } else if (optionChar.symbol == 'b' || optionChar.symbol == 'B') {
            // it's binary number
            kindToken = BinaryNumber;

            TokenizerState_acceptChar(self, optionChar.symbol, optionChar.size);
            optionChar = utf8Iter_peek(self->iter);

            while (optionChar.size != 0)
            {
                uint32_t uchar = optionChar.symbol;

                if (isBinaryDigit(uchar) || uchar == '_') {
                    TokenizerState_acceptChar(self, uchar, optionChar.size);
                } else {
                    break;
                }

                optionChar = utf8Iter_peek(self->iter);
            }

        } else if (optionChar.symbol == 'o' || optionChar.symbol == 'O') {
            // it's octal number
            kindToken = OctalNumber;

            TokenizerState_acceptChar(self, optionChar.symbol, optionChar.size);
            optionChar = utf8Iter_peek(self->iter);

            while (optionChar.size != 0)
            {
                uint32_t uchar = optionChar.symbol;

                if (isOctalDigit(uchar) || uchar == '_') {
                    TokenizerState_acceptChar(self, uchar, optionChar.size);
                } else {
                    break;
                }

                optionChar = utf8Iter_peek(self->iter);
            }
        }

    }
    
    
    while (optionChar.size != 0)
    {
        uint32_t uchar = optionChar.symbol;

        if (isDecimalDigit(uchar) || uchar == '_') {
            TokenizerState_acceptChar(self, uchar, optionChar.size);
        } else if (uchar == '.') {
            uint8_t len = optionChar.size;
        
            TokenizerState_acceptChar(self, uchar, optionChar.size);
            optionChar = utf8Iter_peek(self->iter);
            
            // no chars after dot or is there decimal digit?
            // this means that this is definitely not an access 
            // to a member of a structure/class 
            if (optionChar.size == 0 || isDecimalDigit(optionChar.symbol)) {
                if (kindToken == IntNumber) { kindToken = FloatNumber; }
                else if (kindToken == FloatNumber) {
                    return (TokResult){
                        .isSuccess=false,
                        .value.error=(TokError){
                            .kind=TwoDecimalSeparators,
                            .pos=TokenizerState_getPos(self)
                        }
                    };
                } else if (kindToken == BinaryNumber || kindToken == OctalNumber || kindToken == HexNumber) {
                    return (TokResult){
                        .isSuccess=false,
                        .value.error=(TokError){
                            .kind=NotDecimalFloat,
                            .pos=TokenizerState_getPos(self),
                            .extension=(void*)kindToken
                        }
                    };
                }
            } else {
                TokenizerState_backChar(self, len);
                break;
            }
        } else {
            if (isAlpha(uchar) && kindToken != 0) {
                kindToken += 1; // equivalent: Int -> IntWithPrefix. Float -> FloatWithPrefix...
            }
            break;
        }

        optionChar = utf8Iter_peek(self->iter);
    }
    
    Token token = (Token){
        .kind = kindToken,
        .pos = TokenizerState_getPos(self),
        .content = TokenizerState_getSlice(self)
    };

    return (TokResult){
        .isSuccess=true,
        .value.token = token
    };
}

/* TOKENIZE OPERATOR */
TokResult TokenizerState_acceptOperator(TokenizerState* self) {
    uchar_ITER optionChar = utf8Iter_peek(self->iter);

    if (optionChar.size != 0) {
        size_t kindToken;

        uint32_t firstChar = optionChar.symbol;

        if (firstChar == '@') {
            kindToken = MetaAt;
        } else if (firstChar == '#') {
            kindToken = MetaSharp;
        } else if (firstChar == '$') {
            kindToken = MetaDollar;
        } else if (firstChar == '(') {
            kindToken = lParen;
        } else if (firstChar == ')') {
            kindToken = rParen;
        } else if (firstChar == '[') {
            kindToken = lSquare;
        } else if (firstChar == ']') {
            kindToken = rSquare;
        } else if (firstChar == '{') {
            kindToken = lBracket;
        } else if (firstChar == '}') {
            kindToken = rBracket;
        } else if (firstChar == ',') {
            kindToken = Comma;
        } else if (firstChar == ';') {
            kindToken = SemiColon;
        } else if (firstChar == ':') {
            optionChar = utf8Iter_npeek(self->iter, 1);
                
            if (optionChar.size != 0 && optionChar.symbol == '*') {
                TokenizerState_acceptChar(self, '*', 1);
                kindToken = Colon2;
            } else { kindToken = Colon; }
        } else if (firstChar == '?') {
            kindToken = QuestMark;    
        }
        else {
            if (firstChar == '+') {
                kindToken = Plus;
            } else if (firstChar == '-') {
                kindToken = Minus;
            } else if (firstChar == '*') {
                optionChar = utf8Iter_npeek(self->iter, 1);
                
                if (optionChar.size != 0 && optionChar.symbol == '*') {
                    TokenizerState_acceptChar(self, '*', 1);
                    kindToken = Star2;
                } else { kindToken = Star; }
            } else if (firstChar == '!') {
                kindToken = ExlamMark;
            } else if (firstChar == '=') {
                kindToken = Eq;
            } else if (firstChar == '%') {
                kindToken = Percent;
            } else if (firstChar == '^') {
                kindToken = Caret;
            } else if (firstChar == '~') {
                kindToken = Tilda;
            } else if (firstChar == '&') {
                kindToken = Amp;
            } else if (firstChar == '<') {
                optionChar = utf8Iter_npeek(self->iter, 1);
                
                if (optionChar.size != 0 && optionChar.symbol == '<') {
                    TokenizerState_acceptChar(self, '<', 1);
                    kindToken = rShift;
                } else { kindToken = Less; }
            } else if (firstChar == '>') {
                optionChar = utf8Iter_npeek(self->iter, 1);
                
                if (optionChar.size != 0 && optionChar.symbol == '>') {
                    TokenizerState_acceptChar(self, '>', 1);
                    kindToken = lShift;
                } else { kindToken = Greater; }
            } else {
                // unknown char
                return (TokResult){
                    .isSuccess=false,
                    .value.error=(TokError){
                        .kind=ItsNotOperator,
                        .pos=TokenizerState_getPos(self)
                    }
                };            
            }

            optionChar = utf8Iter_npeek(self->iter, 1);   
            if (optionChar.size != 0 && optionChar.symbol == '=') {
                TokenizerState_acceptChar(self, '=', 1);
                kindToken += 1;
            }
        }

        TokenizerState_acceptChar(self, firstChar, 1);

        Token token = (Token){
            .kind = kindToken,
            .content = TokenizerState_getSlice(self),
            .pos = TokenizerState_getPos(self)
        };

        return (TokResult){
            .isSuccess=true,
            .value.token=token
        };
    } else {
        return (TokResult){
            .isSuccess=false,
            .value.error=(TokError){
                .kind=ItsNotOperator,
                .pos=TokenizerState_getPos(self)
            }
        };   
    }
}

/* TOKENIZE COMMENT */
TokResult TokenizerState_acceptComment(TokenizerState* self) {
    uchar_ITER optionChar = utf8Iter_peek(self->iter);
    TokenizerState_acceptChar(self, optionChar.symbol, optionChar.size);

    if (optionChar.size != 0 && optionChar.symbol == '/') {
        optionChar = utf8Iter_peek(self->iter);

        if (optionChar.size != 0) {
            if (optionChar.symbol == '/') {
                while (optionChar.size != 0)
                {
                    TokenizerState_acceptChar(self, optionChar.symbol, optionChar.size);
                    if (optionChar.symbol == '\n') {
                        return (TokResult){
                            .isSuccess=true,
                            .value.token=(Token){
                                .kind=LineComment,
                                .content=TokenizerState_getSlice(self),
                                .pos=TokenizerState_getPos(self)
                            }
                        };
                    }

                    optionChar = utf8Iter_peek(self->iter);
                }
            } else if (optionChar.symbol == '*') {
                abort();
            }
        }

        optionChar = utf8Iter_peek(self->iter);

        if (optionChar.size != 0 && optionChar.symbol == '=') {
            TokenizerState_acceptChar(self, '/', 1);

            return (TokResult){
                .isSuccess=true,
                .value.token=(Token){
                    .kind=SlashEq,
                }
            };
        }

        return (TokResult){
            .isSuccess=true,
            .value.token=(Token){
                .kind=Slash,
            }
        };
    }
}

TokResult TokenizerState_acceptString(TokenizerState* self) {
    uchar_ITER optionChar = utf8Iter_peek(self->iter);

    if (optionChar.size == 0 || optionChar.symbol != '"') {
        return (TokResult){
            .isSuccess=false,
            .value.error=(TokError){
                .kind=ItsNotString,
            }
        };
    } else {
        TokenizerState_acceptChar(self, optionChar.symbol, optionChar.size);
        optionChar = utf8Iter_peek(self->iter);
    }

    while (optionChar.size != 0) {
        TokenizerState_acceptChar(self, optionChar.symbol, optionChar.size);
        if (optionChar.symbol == '"') {
            break;
        }
        optionChar = utf8Iter_peek(self->iter);
    }

    Token token = (Token){
        .kind=DefString,
        .content=TokenizerState_getSlice(self),
        .pos=TokenizerState_getPos(self)
    };

    return (TokResult){
        .isSuccess=true,
        .value.token=token
    };
}