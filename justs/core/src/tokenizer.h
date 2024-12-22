#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <src/token.h>

enum TokenKind {
    Ident, PrefixString,

    BinaryNumber, BinaryWithPostfix, //BinaryWithDot,
    OctalNumber, OctalWithPostfix, //OctalWithDot,
    HexNumber, HexWithPostfix, //HexWithDot,

    IntNumber, IntWithPostfix, //IntWithDot,
    FloatNumber, FloatWithPostfix, //FloatWithDot,

    Dot,

    MetaAt, MetaSharp, MetaDollar,

    lParen, rParen, lSquare, rSquare, lBracket, rBracket,
    QuestMark, ExlamMark,

    Comma, SemiColon, Colon, Colon2,

    Plus, PlusEq,
    Minus, MinusEq,
    Star, StarEq,
    Slash, SlashEq,
    Star2, Star2Eq,

    Eq, EqEq,
    Tilda, TildaEq,
    Caret, CaretEq,
    Amp, AmpEq,
    Percent, PercentEq,

    Less, LessEq,
    Greater, GreaterEq,
    lShift, lShirtEq,
    rShift, rShiftEq,

    LineComment, MultiLineComment,
    
    DefString, FormatString,

    EoF,
};

enum TokErrorKind {
    /* IDENT ERROR */
    ItsNotIdent, 
    
    /* NUMBER ERROR */
    ItsNotNumber, 
    TwoDecimalSeparators,
    NotDecimalFloat,

    /* OPERATOR ERROR */
    ItsNotOperator,

    /* STRING ERROR*/
    ItsNotString,
};

typedef struct TokenizerState {
    u8Slice source; // text, source code
    u8StringIter iter; // `source` iterator
    size_t curIndex; // index of current character, kind of cursor
    size_t startIndex; // index of first character in current iteration (`next`)
    size_t curLine; 
    size_t curColumn;
    size_t startColumn; // position in line of first character
} TokenizerState;

TokResult TokenizerState_next(TokenizerState* self);

#endif // TOKENIZER_H