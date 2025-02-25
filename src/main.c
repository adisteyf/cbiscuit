#include <stdio.h>
#include "lexer.h"
#include "parser.h"

static char * CODE = "set test 333";


int
main (void)
{
    token_t * list2 = bsqt_parse(CODE);
    token_t * list = list2;

    list2=list2->next;
    puts("list2:");
    for (;list2!=0;list2=list2->next) {
        printf("token: %s\n", list2->value);
    }

    if (!list->next) { puts("head ==NULL"); }
    ast_node_t * node = ast_parse(list);
    if (!node) { puts("node == NULL"); }

    ast_walk(node);
    evaluate(node);
    free_ast(node, 0);
    free_vars();

    return 0;
}
