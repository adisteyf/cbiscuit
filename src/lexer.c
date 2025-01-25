#include "lexer.h"

const size_t bsqt_tokenptr_sizeof = sizeof(BSQTToken *);




void addToken(BSQTTokenArr *token_arr, BSQTToken *token) {
    token_arr->arr = realloc(token_arr->arr, (token_arr->len+1) * bsqt_tokenptr_sizeof);

    token_arr->arr[token_arr->len] = token;
    token_arr->len++;
}

int checkWord(const char * str, int word_len, char * word)
{
    for (unsigned int i=0; i<word_len; ++i) {
        if (str[i] != word[i]) {
            return 0;
        }

        return 1;
    }
}

void free_token(BSQTToken *token) {
    free(token->val);
}

void free_tokenArr(BSQTTokenArr *tokarr) {
    for (size_t i=0; i<tokarr->len; ++i) {
        free_token(tokarr->arr[i]);
        puts("Freed token");
    }
}

int isInteger(const char * str)
{
    for (;*str;) {
        if (!isdigit((unsigned char) *str)) {
            return 0;
        }
        str++;
    }

    return 1;
}

int tokenize(const char * str)
{
    BSQTTokenArr token_arr;
    token_arr.len = 0;

    if (checkWord(str, __BSQT_SET_LEN, __BSQT_SET_STR " ")) { /* space coz we need to check the syntax */
        /* add keyword token */
        BSQTToken token;
        token.type    = __BSQT_KEYWORD_TYPE;
        token.val     = strdup(__BSQT_SET_STR);

        token_arr.arr = malloc((token_arr.len+1) * bsqt_tokenptr_sizeof);
        addToken(&token_arr, &token);


        /* add id (key) token */
        BSQTToken id;
        id.type       = __BSQT_ID_TYPE;

        size_t i = __BSQT_SET_LEN+1;
        size_t id_len=0; 

        char * var_key = malloc(__BSQT_KEYLEN_BYDEF);
        
        for (; str[i+id_len]!=0 && str[i+id_len]!='\n' && str[i+id_len]!=' '; id_len++) {
            var_key[id_len]=str[i+id_len];
        }

        id.val = strdup(var_key);
        free(var_key);
        addToken(&token_arr, &id);



        /* add value token */
        BSQTToken val;
        i += id_len+1;

        char * var_val = malloc(__BSQT_VALLEN_BYDEF);
        size_t val_len=0;
        for (; str[i+val_len]!='\n' && str[i+val_len]!=0; ++val_len) {
            var_val[val_len]=str[i+val_len];
        }
        var_val[val_len] = 0;

        if (isInteger(var_val)) {
            long val_long = atol(var_val);

            val.type = __BSQT_INT_TYPE;
            val.val  = (void *)(intptr_t)val_long;
        }

        else {
            puts("BSQT: Invalid type");
            free(var_val);
            return 1;
        }

        free(var_val);
        addToken(&token_arr, &val);
    }

    printf("key of 1st token is: %s\n", token_arr.arr[0]->val);
    printf("key of 2nd token is: %s\n", token_arr.arr[1]->val);
    printf("key of 3rd token is: %d\n", token_arr.arr[2]->val);
    return 0;
}
