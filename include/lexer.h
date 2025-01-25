#ifndef __BSQT_LEXER_H
#define __BSQT_LEXER_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#define __BSQT_SET_STR  "set"
#define __BSQT_SET_LEN  3
#define __BSQT_SET      10

#define __BSQT_KEYWORD_TYPE 1
#define __BSQT_ID_TYPE      2
#define __BSQT_INT_TYPE     3

#define __BSQT_KEYLEN_BYDEF    15 /* bytes to allocate variable's name */
#define __BSQT_KEYLEN_NEWBYTES 3

#define __BSQT_VALLEN_BYDEF    15 /* bytes to allocate variable's value */
#define __BSQT_VALLEN_NEWBYTES 3





typedef struct {
   void * val;        /* value of token               */
   int    type;       /* type of token (e.g. KEYWORD) */
} BSQTToken;

typedef struct {
    BSQTToken ** arr;
    size_t       len;
} BSQTTokenArr;

typedef struct {
    char * id;
    void * val;
    int    type;
} BSQTVar;

typedef struct {
    BSQTVar ** arr;
    size_t     len;
} BSQTVarArr;


void   addToken  (BSQTTokenArr *token_arr, BSQTToken *token);
int    checkWord (const char * str, int word_len, char * word);
int    tokenize  (const char * str);




#endif /* __BSQT_LEXER_H */
