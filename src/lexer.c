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
    token_arr.len = 0;

    if (checkWord(str, __BSQT_SET_LEN, __BSQT_SET_STR " ")) { /* space coz we need to check the syntax */
        BSQTToken token;
        token.type    = __BSQT_KEYWORD_TYPE;
        token.val     = __BSQT_SET_STR;

        token_arr.arr = malloc((token_arr.len+1) * bsqt_tokenptr_sizeof);
        addToken(&token_arr, &token);

        BSQTToken id;
        id.type       = __BSQT_ID_TYPE;

        size_t i = __BSQT_SET_LEN+1;
        char * var_key = malloc(__BSQT_KEYLEN_BYDEF);
        
        for (size_t j=0; str[i+j]!=0 && str[i+j]!='\n' && str[i+j]!=' '; ++j) {
            printf("%d\n", j);
            var_key[j]=str[i+j];
        }

        id.val = strdup(var_key);
        free(var_key);
        addToken(&token_arr, &id);
    }

    printf("key of 1st token is: %s\n", token_arr.arr[0]->val);
    printf("key of 2nd token is: %s\n", token_arr.arr[1]->val);
}
