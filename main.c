#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    extern int yyparse(void);
    extern FILE *yyin;
    FILE *fp;

    if (argc != 2) {
        fprintf(stderr, "usage:%s filename", argv[0]);
        exit(1);
    }

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "%s not found.\n", argv[1]);
        exit(1);
    }

    yyin = fp;
    if (yyparse()) {
        fprintf(stderr, "Error ! \n");
        exit(1);
    }
}
