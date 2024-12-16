# How tokenization works:

1. The tokenizer is initialized. it **MUST** contain text before it can be used.
2. Higher-level logic uses the tokenizer, requesting one token at a time.

Please note that there are tokens in Just that have no meaning without other tokens. For example, a token of type `IntWithPostfix` is meaningless unless it is followed by an `Ident` token (containing, for example, "i32").

```
123i32 ===tokenization==> IntWithPrefix("123"), Ident("i32")
```

It might be logical to combine such tokens into one object, however, there is only one type of token, which has only two fields - position in the text and string data. Therefore, our implementation passes two different token objects. This fact is taken into account and checked for consistency in the parser.

```c
// the only possible representation of a token
typedef struct Token {
    size_t kind;
    u8Slice content;
    TextPos pos;
} Token;

// This is what a token for language constructs 
// with a prefix could look like
typedef struct TokenWithPrefix {
    Token literal;
    Token prefix;
}

/* however, to avoid dynamic type,traits in the tokenizer, 
a more restrictive approach with a, single token type is used.
on the other hand, the parser will use AST nodes in any case.
These nodes are already dynamic, 
so the merging of tokens occurs at the parsing level. */
```
