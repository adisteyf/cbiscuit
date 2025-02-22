#include "parser.h"
#include "lexer.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


static token_t * head;


ast_node_t *
create_num_node (int val)
{
    ast_node_t * n = (ast_node_t *)malloc(sizeof(ast_node_t));
    n->type=AST_INT;
    n->val=val;

    return n;
}

ast_node_t *
create_bin_node (ast_node_t * left, ast_node_t * right, ast_type_t * type)
{
    ast_node_t * n = (ast_node_t *)malloc(sizeof(ast_node_t));
    n->type=*type;
    n->add.left=left;
    n->add.right=right;

    return n;
}

int
evaluate (ast_node_t * n)
{
    switch (n->type)
    {
        case AST_INT:
            return n->val;
        case AST_PLUS:
            return evaluate(n->add.left)+evaluate(n->add.right);
        case AST_STAR:
            return evaluate(n->add.left)*evaluate(n->add.right);
        case AST_MINUS:
            return evaluate(n->add.left)-evaluate(n->add.right);
        case AST_SLASH:
            return evaluate(n->add.left)/evaluate(n->add.right);
        default:
            return 0;
    }
}

void
free_ast (ast_node_t * n)
{
    if (n->type==AST_PLUS) {
        free_ast(n->add.left);
        free_ast(n->add.right);
    }

    free(n);
}

//ast_node_t *
void
ast_parse (token_t * toks)
{
    toks=toks->next;
    ast_node_t * tmp_left=0;// = create_num_node(1);
    ast_node_t * tmp_right=0;// = create_num_node(2);
    ast_node_t * root=0;// = create_add_note(tmp_left,tmp_right);

    for (; toks!=0;toks=toks->next) {
        if (toks->type==INTEGER) {
            tmp_left=create_num_node(atoi(toks->value));
        }

        else if (toks->type==PLUS) {
            if (tmp_left) {
                tmp_right=create_num_node(atoi(toks->next->value));
                ast_type_t t = AST_PLUS;
                root=create_bin_node(tmp_left, tmp_right, &t);
                tmp_left=root;
                tmp_right=0;
            }
        }

        else if (toks->type==STAR) {
            if (tmp_left) {
                tmp_right=create_num_node(atoi(toks->next->value));
                ast_type_t t = AST_STAR;
                root=create_bin_node(tmp_left, tmp_right, &t);
                tmp_left=root;
                tmp_right=0;
            }
        }

        else if (toks->type==MINUS) {
            if (tmp_left) {
                tmp_right=create_num_node(atoi(toks->next->value));
                ast_type_t t = AST_MINUS;
                root=create_bin_node(tmp_left, tmp_right, &t);
                tmp_left=root;
                tmp_right=0;
            }
        }

        else if (toks->type==SLASH) {
            if (tmp_left) {
                tmp_right=create_num_node(atoi(toks->next->value));
                ast_type_t t = AST_SLASH;
                root=create_bin_node(tmp_left, tmp_right, &t);
                tmp_left=root;
                tmp_right=0;
            }
        }
    }

    if (root!=0) {
        int res =  evaluate(root);
        printf("res: %d\n", res);
        free_ast(root);
    }
    else {
        printf("BSQT: Syntax error.\n");
    }
}
