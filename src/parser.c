#include "parser.h"
#include "lexer.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


ast_node_t *
create_num_node (int val)
{
    ast_node_t * n = (ast_node_t *)malloc(sizeof(ast_node_t));
    n->type=AST_INT;
    n->val=val;

    return n;
}

ast_node_t *
create_bin_node (ast_node_t * left, ast_node_t * right, ast_type_t type)
{
    ast_node_t * n = (ast_node_t *)malloc(sizeof(ast_node_t));
    n->type=type;
    n->add.left=left;
    n->add.right=right;

    return n;
}

int
evaluate (ast_node_t * n)
{
    if (!n) {
        return 0;
    }

    switch (n->type)
    {
        case AST_INT:
            return n->val;
        case AST_PLUS:
            return evaluate(n->add.left) + evaluate(n->add.right);
        case AST_STAR:
            return evaluate(n->add.left) * evaluate(n->add.right);
        case AST_MINUS:
            return evaluate(n->add.left) - evaluate(n->add.right);
        case AST_SLASH:
            return evaluate(n->add.left) / evaluate(n->add.right);
        default:
            return 0;
    }
}

void
free_ast (ast_node_t * n, int isRight)
{
    if (n->type==AST_PLUS||n->type==AST_STAR) {
        free_ast(n->add.left, 0);
        free_ast(n->add.right, 1);
    }

    free(n);
}

ast_type_t
lex2parse (token_type_t * t)
{
    switch (*t)
    {
        case STAR:
            return AST_STAR;
        case SLASH:
            return AST_SLASH;
        case PLUS:
            return AST_PLUS;
        case MINUS:
            return AST_MINUS;
    }
    return 0;
}

ast_node_t *
nums (token_t ** toks) {
    if (*toks == 0 || (*toks)->value == 0) {
        return 0;
    }

    int val = atoi((*toks)->value);
    *toks = (*toks)->next;
    return create_num_node(val);
}

ast_node_t *
factor (token_t ** toks) {
    ast_node_t * n = nums(toks);

    while ((*toks) && (*toks)->type == STAR) {
        token_t * op = *toks;
        *toks = (*toks)->next;
        ast_node_t * right = factor(toks);
        n = create_bin_node(n, right, lex2parse(&op->type));
    }

    return n;
}


ast_node_t *
expression (token_t ** toks) {
    ast_node_t * n = factor(toks);
    while ((*toks) && (*toks)->type == PLUS) {
        token_t * op = *toks;
        *toks = (*toks)->next;
        ast_node_t * right = factor(toks);
        n = create_bin_node(n, right, lex2parse(&op->type));
    }

    return n;
}

void
ast_walk (ast_node_t * n)
{
    if (!n) {
        return;
    }

    if (n->type==AST_INT) {
        printf("%d\n", n->val);
    }

    else if (n->type==AST_PLUS||n->type==AST_STAR) {
        printf("(");
        ast_walk(n->add.left);
        printf(" bin ");
        ast_walk(n->add.right);
        printf(")\n");
    }
}


ast_node_t *
ast_parse (token_t * toks) {
    toks = toks->next;
    ast_node_t * n = expression(&toks);

    if (n) {
        return n;
    }

    return 0;
}
