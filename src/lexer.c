#include <string.h>
#include <stdlib.h>
#define __BSQT_SET_STR  "set"
#define __BSQT_SET_LEN  3
#define __BSQT_SET      10
#define __BSQT_INT_TYPE 1


typedef struct {
   void * val;  /* value of variable           */
   char * key;  /* identifier                  */
   int    type; /* type of variable (e.g. int) */
}; BSQTToken

typedef struct {
    BSQTToken * arr;
    size_t      len;
}; BSQTTokenArr


void addToken(BSQTTokenArr *token_arr, int token) {
    token_arr->len++;
    token_arr->arr = realloc(token_arr->arr, token_arr->len);
    arr[len-1] = token;
}

int checkWord(char * str, int word_len, char * word)
{
    for (unsigned int i=0; i<word_len; ++i) {
        if (str[i] != word_str[i]) {
            return 0;
        }

        return 1;
    }
}

int tokenize(char * str)
{
    BSQTTokenArr token_arr;
    if (checkWord(str, __BSQT_SET_LEN, __BSQT_SET_STR)) {
        addToken(token_arr, __BSQT_SET_STR);
    }

}
