#include "lexer.h"

const size_t bsqt_tokenptr_sizeof = sizeof(BSQTToken *);




void addToken(BSQTTokenArr *token_arr, BSQTToken *token) {
    token_arr->arr = realloc(token_arr->arr, (token_arr->len+1) * bsqt_tokenptr_sizeof);

    token_arr->arr[token_arr->len] = token;
    token_arr->len++;
}

void addVariable(BSQTVarArr *var_arr, BSQTVar *variable) {
    var_arr->arr = realloc(var_arr->variable, (var_arr->len+1) * bsqt_tokenptr_sizeof);

    var_arr->arr[var_arr->len] = variable;
    var_arr->len++;
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

int token_parse(BSQTTokenArr *tokarr) {
    BSQTVarArr vararr;
    BSQTTokenArr token_buff;

    for (size_t i=0; i<tokarr->len; ++i) {
        addToken(&token_buff, tokarr->arr[i]);

        switch (tokarr->arr[i]->type) {
            case __BSQT_KEYWORD_TYPE:
                if (token_buff.len != 1) {
                    puts("BSQT: keyword token isn't in the 1st place.");
                    return 1;
                }

                if (strcmp(tokarr->arr[i]->type, __BSQT_SET_STR)==0) {
                    BSQTVar var;
                    addVariable(&vararr, &var);
                }

                break;
            case __BSQT_ID_TYPE:
                if (token_buff.len != 2 || token_buff.arr[0]->type != __BSQT_KEYWORD_TYPE
                        || strcmp(token_buff.arr[0]->val, __BSQT_SET_STR)==0) { /* check if 1st token isn't "set" */

                    puts("BSQT: syntax error.");
                    return 1;
                }

                /* name of last variable (after set) equals value of current token */

                /********************************************************
                 * [                                                    *
                 *      { token: __BSQT_ID_TYPE, value: "name_of_var" } *
                 * ]                                                    *
                 ********************************************************/

                vararr.arr[vararr.len-1]->id = (char *)token_buff.arr[token_buff.len-1]->val;
                break;
            case __BSQT_INT_TYPE:
                // TODO: add a check
                // TODO: fix all bugs

                vararr.arr[vararr.len-1]->val = token_buff.arr[token_buff.len-1]->val;
                break;
        }
    }

    return 0;
}

int tokenize(const char * str) // TODO: add a cycle for tokenize more than 1 string
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
