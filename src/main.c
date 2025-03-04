#include <stdio.h>
#include "lexer.h"
#include "parser.h"
#include "bsqt_utils.h"

int
main (int argv, char ** argc)
{
    char * CODE=0;
    size_t CODE_SIZE=0;
    biscuit_t * bsqt = (biscuit_t *)malloc(sizeof(biscuit_t));

    if (argv>1) {
        char * old_code = bsqtReadFile(argc[1], &CODE_SIZE);
        CODE = strdup(old_code);
        free(old_code);
    }

    else {
        printf("Biscuit: Too few arguments.\n");
        return 1;
    }

    token_t * list2 = bsqt_parse(CODE);
    token_t * list = list2;

    list2=list2->next;
    puts("list2:");
    for (;list2!=0;list2=list2->next) {
        printf("token: %s\n", list2->value);
    }

    if (!list->next) { puts("head ==NULL"); }
    ast_node_t * node = ast_parse(list, bsqt);
    if (!node) { puts("node == NULL"); }

    ast_walk(node, bsqt);
    evaluate(node);
    free_ast(node, 0);
    free_vars(bsqt);

    return 0;
}
