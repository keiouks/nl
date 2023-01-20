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
    king->line_number = 1;
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
nl_search_variable(Scope *scope, char *identifier) {
    Scope *currentScope;
    Variable *result;
    for (currentScope = scope; currentScope; currentScope = currentScope->upper) {
        for(result = currentScope->variable; result; result = result->next) {
            if(!strcmp(result->name, identifier)) {
                return result;
            }
        }
    }
    return NULL;
}

void
nl_add_variable(Scope *scope, char *identifier, NL_Value *value) {
    /* 先找 */
    Variable *new_var = NULL;
    Variable *temp = NULL;
    for(temp = scope->variable; temp; temp = temp->next) {
        if(!strcmp(temp->name, identifier)) {
            new_var = temp;
            break;
        }
    }
    /* 能找到， 重复声明 */
    if(new_var != NULL) {
        printf("[runtime error] declaring variable [%s] in line [%d] already existed.\n", identifier, nl_get_current_king()->line_number);
        exit(1);
    }
    new_var = malloc(sizeof(Variable));
    new_var->name = malloc(strlen(identifier) + 1);
    strcpy(new_var->name, identifier);
    new_var->next = scope->variable;
    scope->variable = new_var;
    new_var->value = *value;
}

FunctionDefinition *
nl_search_function(Scope *scope, char *name) {
    FunctionDefinition *pos;
    Scope *currentScope;
    for (currentScope = scope; currentScope; currentScope = currentScope->upper) {
        for (pos = currentScope->function_list; pos; pos = pos->next) {
            if (!strcmp(pos->name, name)) {
                return pos;
            }
        }
    }
    return NULL;
}

void
NL_run(King *king) {
    nl_execute_statement_list(king->statement_list, king->scope);
}
