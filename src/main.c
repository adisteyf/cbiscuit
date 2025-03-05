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

    bsqt->toklist = bsqt_parse(CODE);
    free(CODE);
    token_t * list2 = bsqt->toklist;

    list2=list2->next;
    puts("list2:");
    for (;list2!=0;list2=list2->next) {
        printf("token: %s\n", list2->value);
    }

    ast_node_t * node = ast_parse(bsqt);
    if (!node) { puts("node == NULL"); }

    ast_walk(node, bsqt);
    evaluate(node);

    bsqt_var_t * var = bsqtGetVar("test2", bsqt);
    printf("%d\n", var->val);

    free_ast(node, 0);
    free_vars(bsqt);

    return 0;
}
