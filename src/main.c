#include <stdio.h>
#include "lexer.h"
#include "parser.h"

static char * CODE = "3 * 2 + 2 + 7 * 3";


int
main (void)
{
    token_t * list = bsqt_parse(CODE);

    /*for (;list!=0;) {
        printf("token: %s\n", list->value);
        list = list->next;
    }*/

    if (!list->next) { puts("head ==NULL"); }
    ast_parse(list);

    return 0;
}
