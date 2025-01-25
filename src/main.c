#include <stdio.h>
#include "lexer.h"

char * bsqt_code = "set abcdef 123";

int main() {
    tokenize(bsqt_code);
    return 0;
}
