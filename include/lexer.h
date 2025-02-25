#ifndef LEXER_BSQT_H
#define LEXER_BSQT_H

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define BSQT_DEF_SIZE_VARS   10
#define BSQT_DEF_APPEND_VARS 3

typedef enum token_type {
    INTEGER,
    PLUS,
    MINUS,
    SLASH,
    STAR,
    SET,
    ID,
} token_type_t;

typedef struct token {
    token_type_t   type;
    char         * value;
    struct token * next;
} token_t;

token_t * bsqt_parse (char * code);
void      bsqt_push (token_type_t type, char * value);




#endif // LEXER_BSQT_H
