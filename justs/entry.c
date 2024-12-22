#include <stdio.h>

#include <src/tokenizer.h>
#include <src/console.h>
#include <src/filesystem.h>


const unsigned char src[37] = "var x 100i32.sqrt 2 var y 20.32.pow 2";

int main() {
    initConsole();

    FileData fd;
    if (!readFile(L"C:/Users/filli/Рабочий стол/code.just", &fd)) {
        cprint8(u8"Не удалось открыть файл");
        return 0;
    }

    TokenizerState ts = (TokenizerState){
        .source = (u8Slice){
            .data=fd.data,
            .length=fd.size
        },
        .iter=(u8StringIter){
            .first=fd.data,
            .last=fd.data+fd.size
        },
        .startIndex=0,
        .startColumn=0,
        .curLine=0,
        .curIndex=0,
        .curColumn=0
    };

    TokResult tr = TokenizerState_next(&ts);
    while (tr.isSuccess)
    {
        Token token = tr.value.token;
        
        if (token.kind == EoF) break;

        if (token.kind != LineComment && token.kind != MultiLineComment) {
            nprintln8(token.content.data, token.content.length);
        }

        tr = TokenizerState_next(&ts);
    }

    printf("eof\n");
}