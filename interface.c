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
    Scope *scope = malloc(sizeof(Scope));
    scope->function_list = NULL;
    scope->variable = NULL;
    scope->upper = NULL;
    king->statement_list = NULL;
    king->scope = scope;

    return king;
}

void
NL_compile(King *king, FILE *fp) {
    extern int yyparse(void);
    extern FILE *yyin;

    nl_set_current_king(king);
    
    yyin = fp;
    if (yyparse()) {
        fprintf(stderr, "Error ! \n");
        exit(1);
    }
}

Variable *
nl_search_global_variable(King *king, char *identifier) {
    Variable *result;
    for(result = king->scope->variable; result; result = result->next) {
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
    new_var->next = king->scope->variable;
    king->scope->variable = new_var;
    new_var->value = *value;
}

FunctionDefinition *
nl_search_function(char *name) {
    FunctionDefinition *pos;
    King *king = nl_get_current_king();
    for (pos = king->scope->function_list; pos; pos = pos->next) {
        if (!strcmp(pos->name, name)) {
            break;
        }
    }
    return pos;
}

void
NL_run(King *king) {
    nl_execute_statement_list(king->statement_list);
}
