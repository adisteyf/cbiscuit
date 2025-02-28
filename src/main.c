#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

static char * CODE = "set test 333\nset test2 123";

char **
splitString(const char * input, int * count)
{
    char *input_copy = strdup(input);
    if (!input_copy) {
        return NULL;
    }

    char **args = NULL;
    *count = 0;

    char *token = strtok(input_copy, "\n");
    while (token != NULL) {
        args = (char **)realloc(args, (*count + 1) * sizeof(char *));
        if (!args) {
            free(input_copy);
            return NULL;
        }

        args[*count] = strdup(token);
        if (!args[*count]) {
            free(input_copy);
            for (int i = 0; i < *count; i++) {
                free(args[i]);
            }
            free(args);
            return NULL;
        }

        (*count)++;
        token = strtok(NULL, "\n");
    }

    free(input_copy);
    return args;
}

int
main (void)
{
    int codearr_c=0;
    char ** CODE_ARR = splitString(CODE, &codearr_c);

    for (int i=0;i<codearr_c;++i) {
        token_t * list2 = bsqt_parse(CODE_ARR[i]);
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
    }

    return 0;
}
