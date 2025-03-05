#include "parser.h"
#include "lexer.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
free_vars (biscuit_t * bsqt)
{
    bsqt_var_t * current = bsqt->vars;
    while (bsqt->vars_len) {
        bsqt_var_t * next = current->next;
        free(current->id);
        free(current);
        current = next;
        bsqt->vars_len--;
    }
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
nums (token_t ** toks, biscuit_t * bsqt) {
    if ((*toks)->type == NEWLINE) {
        for (;(*toks)->next && (*toks)->next->type == NEWLINE;) {
            *toks = (*toks)->next;
        }

        return 0;
    }

    if (*toks == 0 || (*toks)->value == 0) {
        return 0;
    }

    if ((*toks)->type == SET && (*toks)->next->type == ID) {
        *toks = (*toks)->next;
        bsqt_var_t *var = (bsqt_var_t *)malloc(sizeof(bsqt_var_t));
        var->id = strdup((*toks)->value);
        var->val = 0;
        var->next = bsqt->vars;
        bsqt->vars = var;
        bsqt->vars_len++;
        ast_node_t * n = create_var_node((*toks)->value);
        *toks = (*toks)->next;


        if ((*toks)->type == INTEGER) {
            puts("in if INT");
            bsqt->vars->val = atoi((*toks)->value);
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
factor (token_t ** toks, biscuit_t * bsqt) {
    ast_node_t * n = nums(toks, bsqt);

    while ((*toks) && (*toks)->type == STAR) {
        token_t * op = *toks;
        *toks = (*toks)->next;
        ast_node_t * right = factor(toks, bsqt);
        n = create_bin_node(n, right, lex2parse(&op->type));
    }

    return n;
}

ast_node_t *
expression (token_t ** toks, biscuit_t * bsqt) {
    ast_node_t * n = factor(toks, bsqt);
    while ((*toks) && (*toks)->type == PLUS) {
        token_t * op = *toks;
        *toks = (*toks)->next;
        ast_node_t * right = factor(toks, bsqt);
        n = create_bin_node(n, right, lex2parse(&op->type));
    }

    return n;
}

void
ast_walk (ast_node_t * n, biscuit_t * bsqt)
{
    if (!bsqt->vars4walk) {
        bsqt->vars4walk = bsqt->vars;
    }

    for (;;) {
        if (!n) {
            return;
        }

        if (n->type==AST_INT) {
            printf("%d\n", n->val);
        }

        else if (n->type==AST_PLUS||n->type==AST_STAR) {
            printf("(");
            ast_walk(n->add.left, bsqt);
            printf(" bin ");
            ast_walk(n->add.right, bsqt);
            printf(")\n");
        }

        else if (n->type==AST_VARIABLE) {
            if (!bsqt->vars->val || !bsqt->vars4walk->val) {
                printf("var == NULL\n");
                return;
            }

            printf("%s: %d\n", n->str, bsqt->vars4walk->val);
            bsqt->vars4walk=bsqt->vars->next;
        }

        n=n->next;
    }
}

void
set_var_value (const char * name, int value, biscuit_t * bsqt) /* experemental */
{
    bsqt_var_t *current = bsqt->vars;
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
    new_var->next = bsqt->vars;
    bsqt->vars = new_var;
}

void
endofparse ()
{
    puts("end of parse");
}

ast_node_t *
ast_parse (biscuit_t * bsqt) {
//    set_var_value("abc", 123);

    if (!bsqt->toklist->next) {
        printf("Biscuit: bsqt->toklist == NULL\n");
        return 0;
    }

    bsqt->vars_len=0;
    bsqt->vars4walk=0;
    bsqt->vars = (bsqt_var_t *)malloc(sizeof(bsqt_var_t));
    bsqt->vars->id=0;
    bsqt->vars->val=0;
    bsqt->vars->next=0;

    bsqt->toklist = bsqt->toklist->next;
    int is1stCycle = 1;
    ast_node_t *n = NULL;
    ast_node_t *ast_head = NULL;

    while (bsqt->toklist) {
        ast_node_t *new_node = expression(&bsqt->toklist, bsqt);

        if (is1stCycle) {
            ast_head = new_node;
            n = ast_head;
            is1stCycle = 0;
        } else {
            endofparse();
            n->next = new_node;
            n = n->next;
        }

        if (!bsqt->toklist) {
            break;
        }

        bsqt->toklist = bsqt->toklist->next;
    }

    return ast_head;
}

bsqt_var_t *
bsqtGetVar (const char * name, biscuit_t * bsqt)
{
    bsqt->vars4walk = bsqt->vars;

    for (;;) {
        if (0==strcmp(bsqt->vars4walk->id, name)) {
            return bsqt->vars4walk;
        }

        if (!bsqt->vars4walk->next) {
            return 0;
        }

        bsqt->vars4walk = bsqt->vars4walk->next;
    }
}
