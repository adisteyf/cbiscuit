#include <stdio.h>
#include "lexer.h"

static char * CODE = "set abc 1 + 2";

int
main (void)
{
    token_t * list = bsqt_parse(CODE);

    for (;list!=0;) {
        printf("token: %s\n", list->value);
        list = list->next;
    }

    return 0;
}
