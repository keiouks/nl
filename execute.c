#include "nl.h"
#include <stdlib.h>

void
nl_execute_expression_statement(Statement *statement) {
    nl_eval_expression(statement->u.expression);
}

void
nl_execute_assign_statement(Statement *statement) {
    NL_Value val;
    Variable *var;
    King *king = nl_get_current_king();

    Expression *exp = statement->u.assign.expression;
    char *identifier = statement->u.assign.identifier;

    val = nl_eval_expression(exp);
    var = nl_search_global_variable(king, identifier);

    if(var != NULL) {
        var->value = val;
    } else {
        nl_add_global_variable(king, identifier, &val);
    }
}

void
nl_execute_print_statement(Statement *statement) {
    NL_Value v = nl_eval_expression(statement->u.expression);
    nl_print_value(&v);
}

void
nl_execute_statement(Statement *statement) {
    switch(statement->type) {
        case EXPRESSION_STATEMENT: {
            nl_execute_expression_statement(statement);
            break;
        }
        case ASSIGN_STATEMENT: {
            nl_execute_assign_statement(statement);
            break;
        }
        case PRINT_STATEMENT: {
            nl_execute_print_statement(statement);
            break;
        }
        case STATEMENT_TYPE_PLUS:
        default: {
            printf("[runtime error] in execute statement with unexpected type [%d].\n", statement->type);
            exit(1);
        }
    }
}

void
nl_execute_statement_list(StatementList *list) {
    StatementList *now;
    if (list == NULL) {
        return;
    }
    for (now = list; now; now = now->next) {
        nl_execute_statement(now->statement);
    }
}
