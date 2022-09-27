#include "nl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static King *current_king;

void
nl_set_current_king(King *king) {
    current_king = king;
}

King *
nl_get_current_king(void) {
    return current_king;
}

King *
NL_create_king(void) {
    King *king = malloc(sizeof(struct King_tag));
    king->variable = NULL;

    nl_set_current_king(king);
    return king;
}

void
NL_compile(King *king, FILE *fp) {
    extern int yyparse(void);
    extern FILE *yyin;

    yyin = fp;
    if (yyparse()) {
        fprintf(stderr, "Error ! \n");
        exit(1);
    }
}

Variable *
nl_search_global_variable(King *king, char *identifier) {
    Variable *result;
    for(result = king->variable; result; result = result->next) {
        if(!strcmp(result->name, identifier)) {
            return result;
        }
    }
    return NULL;
}

void
nl_add_global_variable(King *king, char *identifier, NL_Value *value) {
    Variable *new_var = malloc(sizeof(Variable));
    new_var->name = malloc(strlen(identifier) + 1);
    strcpy(new_var->name, identifier);
    new_var->next = king->variable;
    king->variable = new_var;
    new_var->value = *value;
}
