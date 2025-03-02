#include "parser.h"
#include "lexer.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bsqt_var_t * vars;
static bsqt_var_t * vars_head;
int vars_len = 0;
int isVarsHeadSetted = 0;

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

ast_node_t *
create_var_node (const char * name)
{
    ast_node_t * n = (ast_node_t *)malloc(sizeof(ast_node_t));
    n->type=AST_VARIABLE;
    n->str=strdup(name);

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
        case AST_VARIABLE:
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

void
free_vars (void)
{
    bsqt_var_t * current = vars;
    while (vars_len) {
        bsqt_var_t * next = current->next;
        free(current->id);
        free(current);
        current = next;
        vars_len--;
    }

    free(vars);
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
    if (*toks == 0 || (*toks)->value == 0 || (*toks)->type == NEWLINE) {
        return 0;
    }

    if ((*toks)->type == SET && (*toks)->next->type == ID) {
        *toks = (*toks)->next;
        bsqt_var_t *var = (bsqt_var_t *)malloc(sizeof(bsqt_var_t));
        var->id = strdup((*toks)->value);
        var->val = 0;
        var->next = vars;
        vars = var;
        vars_len++;
        ast_node_t * n = create_var_node((*toks)->value);
        *toks = (*toks)->next;


        if ((*toks)->type == INTEGER) {
            puts("in if INT");
            vars->val = atoi((*toks)->value);
            *toks = (*toks)->next;
        }

        return n;
    }

    else if ((*toks)->type == INTEGER) {
        int val = atoi((*toks)->value);
        return create_num_node(val);
    }

    *toks = (*toks)->next;
    return 0;
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
    for (;;) {
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

        else if (n->type==AST_VARIABLE) {
            if (!vars->val) {
                printf("var == NULL\n");
                return;
            }

            printf("%s: %d\n", n->str, vars->val);
        }

        n=n->next;
    }
}

void
set_var_value (const char * name, int value) /* experemental */
{
    bsqt_var_t *current = vars;
    while (current) {
        if (strcmp(current->id, name) == 0) {
            current->val = value;
            return;
        }
        current = current->next;
    }

    bsqt_var_t *new_var = (bsqt_var_t *)malloc(sizeof(bsqt_var_t));
    new_var->id = strdup(name);
    new_var->val = value;
    new_var->next = vars;
    vars = new_var;
}

void
endofparse ()
{
    puts("end of parse");
}

ast_node_t *
ast_parse (token_t * toks) {
//    set_var_value("abc", 123);
    vars = (bsqt_var_t *)malloc(sizeof(bsqt_var_t));
    vars->id=0;
    vars->val=0;
    vars->next=0;
    vars_head=vars;

    toks = toks->next;
    int is1stCycle = 1;
    ast_node_t *n = NULL;
    ast_node_t *ast_head = NULL;

    while (toks) {
        ast_node_t *new_node = expression(&toks);

        if (is1stCycle) {
            ast_head = new_node;
            n = ast_head;
            is1stCycle = 0;
        } else {
            n->next = new_node;
            n = n->next;
        }

        if (!toks) {
            break;
        }

        toks = toks->next;
    }

    endofparse();
    return ast_head;
}
