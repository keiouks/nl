#include "nl.h"
#include <stdlib.h>

StatementValue
nl_execute_expression_statement(Statement *statement) {
    StatementValue sValue;
    sValue.type = NORMAL_STATEMENT_VALUE;
    nl_eval_expression(statement->u.expression);
    return sValue;
}

StatementValue
nl_execute_assign_statement(Statement *statement) {
    StatementValue sValue;
    NL_Value val;
    Variable *var;
    King *king = nl_get_current_king();
    Expression *exp = statement->u.assign.expression;
    char *identifier = statement->u.assign.identifier;

    var = nl_search_global_variable(king, identifier);
    if (var == NULL) {
        printf("[runtime error] assignment with not existed variable [%s].\n", identifier);
        exit(1);
    }

    sValue.type = NORMAL_STATEMENT_VALUE;

    val = nl_eval_expression(exp);

    var->value = val;
    return sValue;
}

StatementValue
nl_execute_declaration_statement(Statement *statement) {
    StatementValue sValue;
    NL_Value val;
    Variable *var;
    King *king = nl_get_current_king();
    Expression *exp = statement->u.declaration.expression;
    char *identifier = statement->u.declaration.identifier;

    var = nl_search_global_variable(king, identifier);
    if (var != NULL) {
        printf("[runtime error] declaring variable [%s] already existed.\n", identifier);
        exit(1);
    }

    sValue.type = NORMAL_STATEMENT_VALUE;

    val = nl_eval_expression(exp);
    nl_add_global_variable(king, identifier, &val);
    return sValue;
}

StatementValue
nl_execute_print_statement(Statement *statement) {
    StatementValue sValue;
    NL_Value v = nl_eval_expression(statement->u.expression);
    sValue.type = NORMAL_STATEMENT_VALUE;
    nl_print_value(&v);
    return sValue;
}

StatementValue
nl_execute_function_definition_statement(Statement *statement) {
    King *king;
    StatementValue sValue;
    FunctionDefinition *newFun;
    char *identifier = statement->u.functionDefinition.identifier;
    Block *block = statement->u.functionDefinition.block;
    FunctionDefinition *functionDefinition = nl_search_function(identifier);

    sValue.type = NORMAL_STATEMENT_VALUE;
    /* 已有同名函数 */
    if (functionDefinition) {
        printf("[runtime error] execute function definition statement while function [%s] is exist.\n", identifier);
        exit(1);
    }
    king = nl_get_current_king();
    newFun = malloc(sizeof(FunctionDefinition));
    newFun->block = block;
    newFun->name = identifier;
    newFun->next = king->function_list;
    king->function_list = newFun;

    return sValue;
}

StatementValue
nl_execute_return_statement(Statement *statement) {
    StatementValue sValue;
    sValue.type = RETURN_STATEMENT_VALUE;
    if (!statement->u.expression) {
        sValue.return_value.type = NULL_VALUE;
        return sValue;
    }
    sValue.return_value = nl_eval_expression(statement->u.expression);
    return sValue;
}

StatementValue
nl_execute_statement(Statement *statement) {
    StatementValue sValue;
    switch(statement->type) {
        case EXPRESSION_STATEMENT: {
            sValue = nl_execute_expression_statement(statement);
            break;
        }
        case ASSIGN_STATEMENT: {
            sValue = nl_execute_assign_statement(statement);
            break;
        }
        case DECLARATION_STATEMENT: {
            sValue = nl_execute_declaration_statement(statement);
            break;
        }
        case PRINT_STATEMENT: {
            sValue = nl_execute_print_statement(statement);
            break;
        }
        case FUNCTION_DEFINITION_STATEMENT: {
            sValue = nl_execute_function_definition_statement(statement);
            break;
        }
        case RETURN_STATEMENT: {
            sValue = nl_execute_return_statement(statement);
            break;
        }
        case STATEMENT_TYPE_PLUS:
        default: {
            printf("[runtime error] execute statement with unexpected type [%d].\n", statement->type);
            exit(1);
        }
    }
    return sValue;
}

StatementValue
nl_execute_statement_list(StatementList *list) {
    StatementList *now;
    StatementValue sValue;
    sValue.type = NORMAL_STATEMENT_VALUE;
    if (list == NULL) {
        return sValue;
    }
    for (now = list; now && (sValue.type == NORMAL_STATEMENT_VALUE); now = now->next) {
        sValue = nl_execute_statement(now->statement);
    }
    return sValue;
}
