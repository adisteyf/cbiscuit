#include "lexer.h"

const size_t bsqt_tokenptr_sizeof = sizeof(BSQTToken *);




void addToken(BSQTTokenArr *token_arr, BSQTToken *token) {
    token_arr->arr = realloc(token_arr->arr, (token_arr->len+1) * bsqt_tokenptr_sizeof);

    token_arr->arr[token_arr->len] = token;
    token_arr->len++;
}

int checkWord(char * str, int word_len, char * word)
{
    for (unsigned int i=0; i<word_len; ++i) {
        if (str[i] != word[i]) {
            return 0;
        }

        return 1;
    }
}

int tokenize(char * str)
{
    BSQTTokenArr token_arr;
    if (checkWord(str, __BSQT_SET_LEN, __BSQT_SET_STR)) {
        BSQTToken token;

        token.type = __BSQT_KEYWORD_TYPE;
        token.key  = "unknown";
        token.val  = (void *)1;

        token_arr.arr = malloc((token_arr.len+1) * bsqt_tokenptr_sizeof);
        addToken(&token_arr, &token);
    }

    printf("key of 1st token is: %s\n", token_arr.arr[0]->key);
}
