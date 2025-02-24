#include <stdio.h>
#include "lexer.h"
#include "parser.h"

static char * CODE = "3 * 2 + 6 * 6";


int
main (void)
{
    token_t * list = bsqt_parse(CODE);

    if (!list->next) { puts("head ==NULL"); }
    ast_node_t * node = ast_parse(list);

    ast_walk(node);
    evaluate(node);
    free_ast(node, 0);

    return 0;
}
