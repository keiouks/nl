#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    extern int yyparse(void);
    extern FILE *yyin;

    yyin = stdin;
    if (yyparse()) {
        fprintf(stderr, "Error ! \n");
        exit(1);
    }
}
