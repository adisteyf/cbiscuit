#include "lexer.h"
#include "parser.h"
#include <string.h>



static token_t * list;
static token_t * head;


void
bsqt_push (token_type_t type, char * value)
{
    list->next = (token_t *)malloc(sizeof(token_t));
    list->next->type = type;
    list->next->value = value;

    list = list->next;
}

int
bsqt_getEq (char * str, char * cmp, size_t pos)
{
    for (int i=0; cmp[i]!=0; ++i) {
        if (str[pos]!=cmp[i]) { return 0; }
        ++pos;
    }

    return 1;
}

token_t *
bsqt_parse (char * code)
{
    if (!code) {
        return 0;
    }

    list = (token_t *)malloc(sizeof(token_t));
    head = list;
    bzero(list, sizeof(token_t));

    for (size_t i=0; i<strlen(code); ++i) {
        char c = code[i];

        /* one-symbol analyzer */
        switch (c)
        {
            case '+': bsqt_push(PLUS, "+"); break;
            case '-': bsqt_push(MINUS, "-"); break;
            case '/': bsqt_push(SLASH, "/"); break;
            case '*': bsqt_push(STAR, "*"); break;
        }

        /* several-symbol analyzer */
        if (bsqt_getEq(code, "set", i)) {
            bsqt_push(SET, "set");
            i+=4;

            char * id = malloc(BSQT_DEF_SIZE_VARS);
            int    j  = 0;

            /* to get an IDENTIFIER */
            for (; code[i]!=' '; ++i) {
                if (id[i]!=0) {
                    id = realloc(id, BSQT_DEF_APPEND_VARS);
                }

                id[j]=code[i];
                ++j;
            }

            bsqt_push(ID, id);
        }

        if (code[i]=='\n') {
            bsqt_push(NEWLINE, "\\n");
        }

        /* integer analyzer */
        if (isdigit(c)) {
            char * ptr = &code[i];
            size_t len = i;
            ++i;
            c = code[i];

            for (;isdigit(c);) {
                ++i;
                c = code[i];
            }

            len = i-len;
            char * value = malloc(len+1);
            strncpy(value, ptr, len);
            value[len]=0;

            bsqt_push(INTEGER, value);
            i--;
        }
    }

    if (!head) { puts("head == NULL in lexer"); }
    return head;
}
