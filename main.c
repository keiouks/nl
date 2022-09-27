#include <stdio.h>
#include <stdlib.h>
#include "_NL.h"

int main(int argc, char **argv) {
    King *king;
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

    king = NL_create_king();

    NL_compile(king, fp);

    return 0;
}
