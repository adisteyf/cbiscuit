#ifndef BSQT_PARSER_H
#define BSQT_PARSER_H

#include "lexer.h"
#include <stdint.h>
#include <stddef.h>

typedef enum ast_type {
    AST_INT,
    AST_PLUS,
    AST_MINUS,
    AST_STAR,
    AST_SLASH,
    AST_ID,
    AST_VARIABLE,
    AST_BINARY,
    AST_UNARY
} ast_type_t;

typedef enum op {
    OP_SET,
    OP_PLUS,
    OP_MINUS,
    OP_MULT,
    OP_DIV
} op_t;

typedef struct bsqt_var {
    char * id;
    int val;
    struct bsqt_var * next;
} bsqt_var_t;

/* Abstract Syntax Tree */
typedef struct ast_node {
    ast_type_t type;
    struct ast_node * next;
    union {
        char * str;
        int val;
        struct {
            struct ast_node * left;
            struct ast_node * right;
        } add;
    };
} ast_node_t;

typedef struct biscuit {
    token_t    * toklist;
    ast_node_t * node;

    bsqt_var_t * vars;
    bsqt_var_t * vars4walk;
    int vars_len;
} biscuit_t;


ast_node_t * create_num_node (int val);
ast_node_t * create_bin_node (ast_node_t *left, ast_node_t *right, ast_type_t type);
int          evaluate        (ast_node_t *n);
void         free_ast        (ast_node_t *n, int isRight);
ast_node_t * ast_parse       (biscuit_t * bsqt);
void         ast_walk        (ast_node_t * n, biscuit_t * bsqt);
ast_node_t * expression      (token_t ** toks, biscuit_t * bsqt);
ast_node_t * factor          (token_t ** toks, biscuit_t * bsqt);
ast_node_t * nums            (token_t ** toks, biscuit_t * bsqt);
void         free_vars       (biscuit_t * bsqt);

#endif // BSQT_PARSER_H
