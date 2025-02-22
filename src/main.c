#include <stdio.h>
#include "lexer.h"
#include "parser.h"

static char * CODE = "set abc 1 + 2";


int
main (void)
{
    token_t * list = bsqt_parse("1 + 2");

    for (;list!=0;) {
        printf("token: %s\n", list->value);
        list = list->next;
    }

    token_t *l = bsqt_parse("1 + 2");

    if (!l->next) { puts("head ==NULL"); }
    ast_parse(l);

    return 0;
}
