# CBiscuit


<p align="center">
    <a href="https://github.com/adisteyf/cbiscuit/blob/main/LICENSE">
        <img src="https://img.shields.io/github/license/adisteyf/cbiscuit?style=for-the-badge">
    </a>
    </br>
    <img src="https://img.shields.io/github/repo-size/adisteyf/cbiscuit?style=for-the-badge">
    <img src="https://img.shields.io/depfu/dependencies/github/adisteyf/cbiscuit?style=for-the-badge" alt="">
</p>


## Introduction
Biscuit is a **Language of Configuration** *(like **YAML**, **TOML**, etc.)* to configure your project.

Syntax of biscuit can described in **one example:**
```bsqt
# comment
set abc 123        # integer
set text "str"     # string
set fltdot 123.456 # double
set boolean true   # bool

# You can change a var using just `ID` & new value
text "new value"
```

## Features
- File extension: **.bsqt**
- Abbreviation: *bsqt*
- Clean and minimal syntax

## Example

To use **Biscuit** parser you need to watch the simplest example:
```c
#include <stdio.h>
#include "lexer.h"
#include "parser.h"

static char * CODE = "set abc 123\nset test2 111";

int
main (int argv, char ** argc)
{
	/* alloc. mem. 4 bsqt */
    biscuit_t * bsqt = (biscuit_t *)malloc(sizeof(biscuit_t));

	/* tokenize & parse */
    bsqt->toklist = bsqt_parse(CODE); 
    ast_node_t * node = ast_parse(bsqt); 

	/* print the AST tree (optional) & solve math e.g. 321-123 */
    ast_walk(node, bsqt); 
    evaluate(node);

	/* get a variable */
    bsqt_var_t * var = bsqtGetVar("abc", bsqt);
    printf("%d\n", var->val);

	/* free memory */
    free_ast(node, 0);
    free_vars(bsqt);

    return 0;
}
```

---
Currently **CBiscuit** in *beta-testing*.

